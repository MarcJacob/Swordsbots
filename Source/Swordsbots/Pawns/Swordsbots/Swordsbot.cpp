// Fill out your copyright notice in the Description page of Project Settings.


#include "Swordsbot.h"
#include "Engine/CollisionProfile.h"
#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "../../Animation/SwordsbotAnimInstance.h"
#include "States/SwordsbotState.h"
#include "States/SwordsbotDashingState.h"
#include "States/SwordsbotRollingState.h"

ASwordsbot::ASwordsbot(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer.SetDefaultSubobjectClass<USwordsbotMovementComponent>(TEXT("Mob Movement Component")))
{
}

void ASwordsbot::BeginPlay()
{
	Super::BeginPlay();
}

void ASwordsbot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASwordsbot::MoveTowards(FVector Direction)
{
	if (Direction.IsNearlyZero()) return;
	if (GetCurrentState() != nullptr && GetCurrentState()->DisallowsMovement()) return;
	FVector finalInput = Direction;
	if (bIsInCombatStance) finalInput *= CombatStanceMovementSpeedModifier;
	AddMovementInput(finalInput);
}

void ASwordsbot::SetIsSprinting(bool Sprinting)
{
	if (Sprinting)
	{
		GetSwordsbotMovementComponent()->StartSprinting();
	}
	else
	{
		GetSwordsbotMovementComponent()->StopSprinting();
	}
}

USwordsbotDashingState* ASwordsbot::Dash(FVector2D Direction)
{
	if (GetCurrentState() != nullptr && GetCurrentState()->DisallowsDashing()) return nullptr;

	GetSwordsbotAnimation()->UpdateSpeedParameters(0.f, 0.f);
	Direction.Normalize();

	USwordsbotDashingState* dashingState = NewObject<USwordsbotDashingState>();
	dashingState->SetDashProperties(DashDistance / DashTime, DashTime, FVector(Direction.X, Direction.Y, 0.f));
	SetNextState(dashingState);
	return dashingState;
}

USwordsbotRollingState* ASwordsbot::Roll(FVector2D Direction)
{
	if (GetCurrentState() != nullptr && GetCurrentState()->DisallowsDashing()) return nullptr;

	GetSwordsbotAnimation()->UpdateSpeedParameters(0.f, 0.f);
	Direction.Normalize();

	USwordsbotRollingState* rollingState = NewObject<USwordsbotRollingState>();
	rollingState->SetRollProperties(RollDistance / RollTime, RollTime, FVector(Direction.X, Direction.Y, 0.f), RollInvulnerabilityTime);
	SetNextState(rollingState);
	return rollingState;
}

void ASwordsbot::SetIsInCombatStance(bool CombatStance)
{
	if (GetCurrentState() != nullptr && GetCurrentState()->DisallowsStanceSwitch()) return;
	bIsInCombatStance = CombatStance;
	GetSwordsbotAnimation()->SetIsInCombatStance(bIsInCombatStance);
	
	if (CombatStance) CombatComponent->EnableBlocking();
	else CombatComponent->DisableBlocking();
}

void ASwordsbot::SetGuard(bool GuardingDown, bool GuardingRight)
{
	GetSwordsbotAnimation()->SetGuardDirection(GuardingDown, GuardingRight);
	EGuardDirection newGuardDirection = static_cast<EGuardDirection>((2 * GuardingDown) + (GuardingRight));
	
	if (newGuardDirection != CombatComponent->CurrentGuardDirection)
	{
		CombatComponent->CurrentGuardDirection = newGuardDirection;
		CombatComponent->ResetBlockingTime();
	}
	
}
