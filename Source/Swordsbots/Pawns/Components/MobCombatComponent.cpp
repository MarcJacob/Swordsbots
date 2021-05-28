// Fill out your copyright notice in the Description page of Project Settings.


#include "MobCombatComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Particles/ParticleSystem.h>
#include "../../GameInstance/SoundGameSubsystem.h"
#include <Sound/SoundCue.h>
#include "../Mob_Base.h"

// Sets default values for this component's properties
UMobCombatComponent::UMobCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	bCanBeTargeted = true;
	// ...
}

void UMobCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerMob = Cast<AMob_Base>(GetOwner());
	if (OwnerMob == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR : Attempted to assign a Mob Combat Component to a non-Mob Actor !"));
		GetOwner()->Destroy();
	}
}

void UMobCombatComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (CanBlock())
	{
		BlockingTime += DeltaTime;
	}
}

EHitResponse UMobCombatComponent::ProcessStrike(AActor* Target, EGuardDirection HitDirection, bool IsBlockable, bool IsParriable, int DamageAmount)
{
	FDamageEvent damageEvent;
	float damageAmount = DamageAmount;
	
	UMobCombatComponent* targetCombatComponent = Cast<UMobCombatComponent>(Target->GetComponentByClass(UMobCombatComponent::StaticClass()));
	
	if (!targetCombatComponent)
	{
		// If the target has no combat component, then the hit just goes through and activates whatever implementation of damage taking the target has.
		Target->TakeDamage(damageAmount, damageEvent, nullptr, GetOwner());
		return EHitResponse::HIT_TAKEN;
	}
	else
	{
		// Let the target process the strike and communicate back whether the hit was blocked, parried or taken.
		EHitResponse hitResponse = targetCombatComponent->ReceiveStrike(this, HitDirection, IsBlockable, IsParriable, DamageAmount);
		return hitResponse;
	}
}

EHitResponse UMobCombatComponent::ReceiveStrike(UMobCombatComponent* Attacker, EGuardDirection AttackDirection, bool IsBlockable, bool IsParriable, int DamageAmount)
{
	EHitResponse hitResponse;

	// Calculate front to attacker angle and determine whether the angle is positive or negative, from 0 to 180.
	// From this and the current guarding direction we can determine whether the hit was blocked.
	FVector toAttacker = (Attacker->GetOwner()->GetActorLocation() - GetOwner()->GetActorLocation()).GetUnsafeNormal();
	FVector myForward = GetOwner()->GetActorForwardVector().GetUnsafeNormal();
	float frontToAttackerAngle = (1 - FVector::DotProduct(myForward, toAttacker)) * 90;
	if (FVector::DotProduct(toAttacker, GetOwner()->GetActorRightVector()) < 0)
	{
		// We decide that negative angle means the attacker is on the left. Thus if dot product of owner's right vector and "to attacker" vector is negative, then attacker is on the left and the angle should be negative.
		frontToAttackerAngle = -frontToAttackerAngle;
	}


	bool hitDeflected = false;
	// BLOCKING & PARRYING
	if (CanBlock())
	{
		// Determine whether we can block.
		bool blockSuccessful = false;

		if (!bOmniDirectionalBlocking)
		{
			// Left and Right hits can be blocked by the "wrong" side under some circumstances but never up or down. Thus check top and bottom blocking first.
			// If top / bottom match, then use the attack angle and left / right to determine block success.

			// Top / Bottom is determined by second bit.
			bool topBottomMatch = (static_cast<uint8>(CurrentGuardDirection) & 2) == (static_cast<uint8>(AttackDirection) & 2);

			if (topBottomMatch)
			{
				// Determine which guards are acceptable to block with. Can be both, or neither.

				// NOTE : Left / Right is determined by first bit.
				bool attackIsRightDir = (static_cast<uint8>(AttackDirection) & 1);

				bool canBlockWithRightGuard = frontToAttackerAngle < 80 && // Can't block targets too far on the side
					(attackIsRightDir && (frontToAttackerAngle > 15) // "Wrong" block that works when attacker is some way off to the right
						|| (!attackIsRightDir && (frontToAttackerAngle > -30 && frontToAttackerAngle < 45))); // "Correct" block that works when relatively close to facing attacker

				bool canBlockWithLeftGuard = frontToAttackerAngle > -80 && // Can't block targets too far on the side
					(attackIsRightDir && (frontToAttackerAngle < 30 && frontToAttackerAngle > -45) // "Correct" block that works when relatively close to facing attacker
						|| (!attackIsRightDir && (frontToAttackerAngle < -15))); // "Wrong" block that works when attacker is some way off to the left

				blockSuccessful = canBlockWithRightGuard && (static_cast<uint8>(CurrentGuardDirection) & 1)
					|| canBlockWithLeftGuard && (~static_cast<uint8>(CurrentGuardDirection) & 1);
			}
		}
		else
		{
			// With Omni Directional Blocking, blocking works as if we were in all guards at once.
			// Angle of tolerance depends on whether we do an "inward" or "outward" attack (left attack from the right for example).

			bool isOutwardAttack = (static_cast<uint8>(AttackDirection) & 1) && frontToAttackerAngle < 0
				|| (static_cast<uint8>(AttackDirection) & 0) && frontToAttackerAngle > 0;

			float absoluteAngle = UKismetMathLibrary::Abs(frontToAttackerAngle);
			blockSuccessful = (isOutwardAttack && absoluteAngle < 60) || (!isOutwardAttack && absoluteAngle < 90);
		}


		if (blockSuccessful)
		{
			// Should this be a parry or a block ?
			if (IsParriable && BlockingTime < ParryTime) // Parry
			{
				// Play FXs
				{
					FVector inBetweenPosition = (GetOwner()->GetActorLocation() + Attacker->GetOwner()->GetActorLocation()) / 2 + CombatFXOffset;
					GetWorld()->GetGameInstance()->GetSubsystem<USoundGameSubsystem>()->PlayFXSound(OnParriedSounds[UKismetMathLibrary::RandomIntegerInRange(0, OnParriedSounds.Num() - 1)], inBetweenPosition);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnParriedParticleSystem, inBetweenPosition, FQuat::Identity.Rotator());
				}


				hitResponse = EHitResponse::HIT_PARRIED;
			}
			else if (IsBlockable) // Block
			{
				// Play FXs
				{
					FVector inBetweenPosition = (GetOwner()->GetActorLocation() + Attacker->GetOwner()->GetActorLocation()) / 2 + CombatFXOffset;
					GetWorld()->GetGameInstance()->GetSubsystem<USoundGameSubsystem>()->PlayFXSound(OnBlockedSounds[UKismetMathLibrary::RandomIntegerInRange(0, OnBlockedSounds.Num() - 1)], inBetweenPosition);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnBlockedParticleSystem, inBetweenPosition, FQuat::Identity.Rotator());
				}

				hitResponse = EHitResponse::HIT_BLOCKED;
			}

			hitDeflected = true;
		}
	}
	// TAKING
	if (!hitDeflected)
	{
		hitResponse = EHitResponse::HIT_TAKEN;
		// Play FXs
		{
			FVector inBetweenPosition = GetOwner()->GetActorLocation() + (Attacker->GetOwner()->GetActorLocation() - GetOwner()->GetActorLocation()) / 10.f + CombatFXOffset;
			GetWorld()->GetGameInstance()->GetSubsystem<USoundGameSubsystem>()->PlayFXSound(OnStruckSounds[UKismetMathLibrary::RandomIntegerInRange(0, OnStruckSounds.Num() - 1)], inBetweenPosition);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnStruckParticleSystem, inBetweenPosition, FQuat::Identity.Rotator());
		}
	}
	
	OnStruckInMelee.Broadcast(Attacker, DamageAmount, hitResponse);
	return hitResponse;

}

void UMobCombatComponent::DetectHittableTargets(TArray<HittableTargetScanResult>& HittableTargets)
{
	// TODO : Come up with smarter way to detect nearby Pawns.
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>();
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> actorsToIgnore = TArray<AActor*>();
	actorsToIgnore.Add(GetOwner());

	TArray<AActor*> actorsScanResults = TArray<AActor*>();

	if (UKismetSystemLibrary::BoxOverlapActors(GetWorld(), GetOwner()->GetActorLocation(), HitRange * FVector::OneVector, objectTypes, nullptr, actorsToIgnore, actorsScanResults))
	{
		FVector ownerForwardVector = GetOwner()->GetActorForwardVector();

		// Iterate through array of potential targets backwards to avoid the problem of invalid indexes when removing an element.
		for (int potentialTargetIndex = actorsScanResults.Num() - 1; potentialTargetIndex >= 0; potentialTargetIndex--)
		{
			AActor* potentialTarget = actorsScanResults[potentialTargetIndex];
			AMob_Base* potentialTargetMob = Cast<AMob_Base>(potentialTarget);
			// If we don't deal friendly fire, is the potential target an enemy ?
			if (potentialTargetMob == nullptr || bFriendlyFireEnabled || OwnerMob->IsEnemyOf(potentialTargetMob))
			{
				// Is potential target within hit angle ?

				FVector ownerToTarget = potentialTarget->GetActorLocation() - GetOwner()->GetActorLocation();
				float normalizedFrontToTargetDotProduct = FVector::DotProduct(ownerForwardVector.GetUnsafeNormal(), ownerToTarget.GetUnsafeNormal());

				float frontToTargetAngle = 90 * (1.f - normalizedFrontToTargetDotProduct);
				if (frontToTargetAngle <= HitAngle)
				{
					HittableTargets.Add(HittableTargetScanResult(potentialTarget, ownerToTarget.Size()));
				}
			}
		}
	}

	HittableTargets.Sort();
}

