// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFighterMobAI.h"
#include "../../../Pawns/Mob_Base.h"
#include <Kismet/KismetMathLibrary.h>
#include "../MobAIStatics.h"

void ASimpleFighterMobAI::OnPossess(APawn* Possessed)
{
	Super::OnPossess(Possessed);
	for (FMobAIPattern& pattern : CombatPatterns)
	{
		pattern.AssignControlledMob(GetControlledMob());
		pattern.Reset();
	}
}

void ASimpleFighterMobAI::Tick(float DeltaTime)
{
	bool targetIsValid = Target != nullptr && IsValid(Target);

	if (targetIsValid)
	{
		AMob_Base* targetMob = Cast<AMob_Base>(Target);
		if (targetMob != nullptr)
		{
			targetIsValid = targetMob->IsAlive();
		}
	}


	if (!targetIsValid)
	{
		OutOfCombatTick(DeltaTime);
	}
	else
	{
		// Determine if we're in combat range. If we're too far or too close, adjust.
		MovementToTargetTick(DeltaTime);
		if (bIsInCombatRange)
		{
			InCombatRangeTick(DeltaTime);
		}
	}
}

void ASimpleFighterMobAI::OutOfCombatTick(float DeltaTime)
{
	// TODO : Implement wandering behavior ?
	GetControlledMob()->UnlockRotation();
	AActor* newTarget = MobAIStatics::FindClosestTargetFor(GetControlledMob(), 800.f);
	if (newTarget != nullptr)
	{
		ChangeTarget(newTarget);
	}
}

void ASimpleFighterMobAI::MovementToTargetTick(float DeltaTime)
{
	FVector ToTarget = Target->GetActorLocation() - GetControlledMob()->GetActorLocation();
	float distanceToTarget = ToTarget.Size();
	bIsInCombatRange = distanceToTarget < CombatDistance;

	GetControlledMob()->LockRotationTo(ToTarget.Rotation());

	if (!bIsInCombatRange)
	{
		// We are out of range - get closer
		GetControlledMob()->MoveTowards(ToTarget.GetUnsafeNormal());
	}
	else if (CurrentCombatPatternIndex < 0 && distanceToTarget < CombatDistance * 0.5f)
	{
		// We are too close and not busy attacking - get further away
		GetControlledMob()->MoveTowards(-ToTarget.GetUnsafeNormal());
	}
}

void ASimpleFighterMobAI::InCombatRangeTick(float DeltaTime)
{
	if (CurrentCombatPatternIndex < 0)
	{
		TimeUntilNextCombatPattern += DeltaTime;
		if (TimeUntilNextCombatPattern > TimeBetweenCombatPatterns)
		{
			CurrentCombatPatternIndex = UKismetMathLibrary::RandomIntegerInRange(0, CombatPatterns.Num() - 1);
		}
		else return; // ------ Mob Controller is waiting before performing next pattern - return now.
	}

	CombatPatterns[CurrentCombatPatternIndex].UpdatePattern(DeltaTime);
	if (CombatPatterns[CurrentCombatPatternIndex].IsDone())
	{
		GetControlledMob()->SetActorRotation((GetTarget()->GetActorLocation() - GetControlledMob()->GetActorLocation()).Rotation());
		CombatPatterns[CurrentCombatPatternIndex].Reset();
		CurrentCombatPatternIndex = -1;
		TimeUntilNextCombatPattern = 0.f;
	}
}

void ASimpleFighterMobAI::ChangeTarget(AActor* NewTarget)
{
	AActor* oldTarget = Target;
	Target = NewTarget;
	TargetChangedDelegate.Broadcast(oldTarget, Target);
}