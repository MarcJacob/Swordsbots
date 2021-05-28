// Fill out your copyright notice in the Description page of Project Settings.


#include "Mob_Base.h"
#include "MobState.h"
#include "States/MobAerialState.h"
#include "States/MobStunUntilNotifyState.h"
#include "States/MobDeadState.h"
#include "States/MobAttackingState.h"

#include <Components/CapsuleComponent.h>
#include "Engine/CollisionProfile.h"

#include "../GameInstance/AllegianceGameSubsystem.h"

AMob_Base::AMob_Base(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Combat Component
	CombatComponent = CreateDefaultSubobject<UMobCombatComponent>(TEXT("Mob Combat Component"));
	MovementComponent = CreateDefaultSubobject<UMobMovementComponent>(TEXT("Mob Movement Component"));

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Mob Collision Component"));
	SetRootComponent(CollisionComponent);

	CollisionComponent->InitCapsuleSize(34.0f, 88.0f);
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetShouldUpdatePhysicsVolume(true);
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->bDynamicObstacle = true;
	CollisionComponent->bEditableWhenInherited = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mob Mesh Component"));
	MeshComponent->SetupAttachment(GetRootComponent());

	MeshComponent->AlwaysLoadOnClient = true;
	MeshComponent->AlwaysLoadOnServer = true;
	MeshComponent->bOwnerNoSee = false;
	MeshComponent->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	MeshComponent->bCastDynamicShadow = true;
	MeshComponent->bAffectDynamicIndirectLighting = true;
	MeshComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
	MeshComponent->SetCollisionProfileName(MeshCollisionProfileName);
	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->SetCanEverAffectNavigation(false);

	MobName = FName(GetName());
}

void AMob_Base::PostInitializeComponents()
{
	AnimationInstance = Cast<UMobAnimInstance>(GetMesh()->GetAnimInstance());
	Super::PostInitializeComponents();
}

void AMob_Base::BeginPlay()
{
	Super::BeginPlay();
	// Callbacks setup

	CombatComponent->OnStruckInMelee.AddDynamic(this, &AMob_Base::OnStruckInMelee);

	// Default state
	CurrentHealth = MaxHealth;

	// Update appearance depending on allegiance.
	Faction factionData = GetGameInstance()->GetSubsystem<UAllegianceGameSubsystem>()->GetFactionData(FactionID);

	if (factionData.FactionColor != nullptr)
	{
		GetMesh()->SetMaterial(1, factionData.FactionColor);
	}

	GetMovementComponent()->AssignRootMotionSourceMesh(MeshComponent);
}

void AMob_Base::Tick(float DeltaTime)
{

	// State machine management
	if (NextState != CurrentState)
	{
		// Switch to next state
		if (CurrentState != nullptr)
		{
			CurrentState->OnEnd();
		}

		
		if (NextState != nullptr)
		{
			NextState->BasicInitialization(this);
			NextState->OnBegin();
		}
		CurrentState = NextState;
	}

	// When no state is currently active, check if we're falling and start the Aerial state next frame if we are.
	if (!HasState() && GetMovementComponent()->IsFalling())
	{
		SetNextState(NewObject<UMobAerialState>());
	}

	if (CurrentState != nullptr)
	{
		CurrentState->OnUpdate(DeltaTime);
		if (CurrentState->HasEnded())
		{
			SetNextState(nullptr);
		}
	}

	if (CurrentState == nullptr || !CurrentState->OverridesDefaultBehavior())
	{
		FVector2D groundedMovementLocal = MovementComponent->GetGroundedNormalizedMovement().GetRotated(-GetActorRotation().Yaw);
		FVector2D groundedMovement = MovementComponent->GetGroundedNormalizedMovement();
		FVector lastInputVector = MovementComponent->GetLastInputVector();
		if (groundedMovementLocal.IsNearlyZero())
		{
			AnimationInstance->UpdateSpeedParameters(0.f, 0.f);
		}
		else
		{
			AnimationInstance->UpdateSpeedParameters(groundedMovementLocal.X, groundedMovementLocal.Y);
		}

		if (bIsRotationLocked)
		{
			SetActorRotation(LockedRotation);
		}
		else if (groundedMovement.IsNearlyZero() == false)
		{
			SetActorRotation(FVector(groundedMovement, 0).Rotation());
		}
	}
	else
	{
		AnimationInstance->UpdateSpeedParameters(0.f, 0.f);
	}
}

void AMob_Base::SetNextState(UMobState* Next)
{
	NextState = Next;
}

float AMob_Base::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* DamageInstigator, AActor* DamageCauser)
{
	AnimationInstance->OnHurtFront(); // TODO : Somehow play a different event depending on, or pass in the direction we were hurt from.
	CurrentHealth -= DamageAmount;
	if (CurrentHealth > 0)
	{
		if (bStunOnHit)
		{
			UMobStunUntilNotifyState* stunState = NewObject<UMobStunUntilNotifyState>();
			stunState->SetNotifyName("HurtReaction_End");
			stunState->SetStunnedVelocity((GetActorLocation() - DamageCauser->GetActorLocation()).GetUnsafeNormal() * 500.f);;
			SetNextState(stunState);
		}

		OnHurtDelegate.Broadcast(DamageAmount);
	}
	else
	{
		// Death
		UMobDeadState* deadState = NewObject<UMobDeadState>();
		deadState->SetDespawnTime(DespawnTimeAfterDeath);
		deadState->SetTurnToRagdoll(bRagdollOnDeath);
		SetNextState(deadState);
		Kill();
		OnDeathDelegate.Broadcast();
	}

	return DamageAmount;
}


// ROOM MOB IMPLEMENTATION

bool AMob_Base::IsDefeated()
{
	return !IsAlive();
}

void AMob_Base::Jump()
{
	if (MovementComponent->IsFalling() == false && (CurrentState == nullptr || !CurrentState->DisallowsMovement()))
	{
		SetNextState(NewObject<UMobAerialState>());
		MovementComponent->Jump();
	}

}

UMobState* AMob_Base::Attack()
{
	if (CurrentState != nullptr && CurrentState->DisallowsCombatActions()) return nullptr;

	UMobAttackingState* attackingState = NewObject<UMobAttackingState>();
	attackingState->SetAttackDirection(CombatComponent->CurrentGuardDirection);
	SetNextState(attackingState);

	return attackingState;
}

// Combat Component
void AMob_Base::OnStruckInMelee(UMobCombatComponent* Striker, float Damage, EHitResponse HitResponse)
{
	if (HitResponse == EHitResponse::HIT_TAKEN)
	{
		TakeDamage(Damage, FDamageEvent(), Striker->GetOwner()->GetInstigatorController(), Striker->GetOwner());
	}
	else
	{
		AnimationInstance->OnBlocking();
	}
}

void AMob_Base::MoveTowards(FVector Direction)
{
	if (Direction.IsNearlyZero()) return;
	if (CurrentState != nullptr && CurrentState->DisallowsMovement()) return;
	SetActorRotation(FRotator(0, Direction.Rotation().Yaw, 0));
	AddMovementInput(Direction);
}

void AMob_Base::LockRotationTo(FRotator Rotation)
{
	Rotation.Pitch = 0;
	Rotation.Roll = 0;
	LockedRotation = Rotation;
	bIsRotationLocked = true;
}

void AMob_Base::UnlockRotation()
{
	bIsRotationLocked = false;
}