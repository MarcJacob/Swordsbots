// Fill out your copyright notice in the Description page of Project Settings.


#include "FirewallBossController.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include "../../../../Pawns/Swordsbots/Swordsbot.h"
#include "../../../../Pawns/MobState.h"

void AFirewallBossController::OnControlledBossPhaseChange(int NewPhase)
{	
	CurrentPhase = NewPhase;

	if (NewPhase == 0)
	{
		UMobStunUntilNotifyState* kneltState = ControlledFirewall->Kneel();
	}
	else if (NewPhase == 1)
	{
		ControlledFirewall->StandUp();
	}

}

void AFirewallBossController::OnPossess(APawn* Possessed)
{
	ControlledFirewall = Cast<AFirewall_Mob_Base>(Possessed);
	Super::OnPossess(Possessed);
	
	Target = Cast<ASwordsbot>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Target->OnDeathDelegate.AddDynamic(this, &AFirewallBossController::OnTargetDeath);
	if (ControlledFirewall == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR : Attempted to assign a Firewall Boss Controller to a non Firewall Pawn."));
	}

	for (FMobAIPattern& pattern : MeleeRangeCombatPatterns)
	{
		pattern.AssignControlledMob(GetControlledMob());
		pattern.Reset();
	}
	for (FMobAIPattern& pattern : LongRangeCombatPatterns)
	{
		pattern.AssignControlledMob(GetControlledMob());
		pattern.Reset();
	}
}

void AFirewallBossController::AssignPossibleJumpPositions(TArray<UPrimitiveComponent*> JumpZones)
{
	PossibleJumpZones = JumpZones;
}

FVector AFirewallBossController::GetSuitableJumpPosition(FVector FromPosition)
{
	// Cycle through every assigned zone, query them for closest point to FromPosition and choose whichever zone returned the
	// point closest to FromPosition.

	FVector closestPoint;
	float closestPointDistance = PossibleJumpZones[0]->GetClosestPointOnCollision(FromPosition, closestPoint);

	for (int zoneID = 1; zoneID < PossibleJumpZones.Num(); zoneID++)
	{
		FVector point;
		float dist = PossibleJumpZones[zoneID]->GetClosestPointOnCollision(FromPosition, point);
		if (dist < closestPointDistance)
		{
			closestPointDistance = dist;
			closestPoint = point;
		}
	}

	return closestPoint;
}

void AFirewallBossController::Tick(float DeltaTime)
{
	switch (CurrentPhase)
	{
	case(0):
		// Do nothing
		break;
	case(1):
		ControlledFirewall->SwitchOnGuardMode(true);
		CurrentPhase = 2;
		CurrentCombatPatternIndex = 0;
		UsingLongRangePattern = true; // Hardcode an immediate use of the first long range combat pattern
		break;
	case(2):
		// Get closer to target if too far from long range attack distance and melee attack distance.
		ManageDistanceToTarget(DeltaTime);
		ManageCombatPatterns(DeltaTime);
		break;
	}
}

void AFirewallBossController::ManageDistanceToTarget(float DeltaTime)
{
	FVector toTarget = Target->GetActorLocation() - GetControlledMob()->GetActorLocation();
	toTarget.Z = 0.f;
	float distanceToTarget = toTarget.Size();
	toTarget.Normalize();

	// Allow movement if not currently in a combat pattern.
	if (CurrentCombatPatternIndex < 0)
	{
		if ((distanceToTarget > LongRangeAttackDistance || distanceToTarget < LongRangeAttackDistance / 2) && distanceToTarget > MeleeRangeAttackDistance * 0.9f)
		{
			GetControlledMob()->MoveTowards(toTarget);
		}
		else if (distanceToTarget > MeleeRangeAttackDistance * 0.9f)
		{
			GetControlledMob()->MoveTowards(-toTarget);
		}
	}


	GetControlledMob()->LockRotationTo(toTarget.Rotation());
}

void AFirewallBossController::ManageCombatPatterns(float DeltaTime)
{
	if (CurrentCombatPatternIndex < 0)
	{
		TimeUntilNextCombatPattern -= DeltaTime;
		if (TimeUntilNextCombatPattern <= 0.f)
		{
			ChooseNextCombatPattern();
		}
		
		if (CurrentCombatPatternIndex < 0) return; // ------ Mob Controller is waiting before performing next pattern or isn't at a suitable range - return now.
	}

	FMobAIPattern* chosenPattern;
	if (UsingLongRangePattern)
	{
		chosenPattern = &LongRangeCombatPatterns[CurrentCombatPatternIndex];
	}
	else
	{
		chosenPattern = &MeleeRangeCombatPatterns[CurrentCombatPatternIndex];
	}
	chosenPattern->UpdatePattern(DeltaTime);
	if (chosenPattern->IsDone())
	{
		chosenPattern->Reset();
		CurrentCombatPatternIndex = -1;
		TimeUntilNextCombatPattern = AverageTimeBetweenCombatPatterns + UKismetMathLibrary::RandomFloatInRange(-TimeBetweenCombatPatternsRandomVariance, TimeBetweenCombatPatternsRandomVariance);
	}
}

void AFirewallBossController::ChooseNextCombatPattern()
{
	// Don't choose anything if the controlled mob is currently in a combat actions blocking state
	if (GetControlledMob()->GetCurrentState() != nullptr && GetControlledMob()->GetCurrentState()->DisallowsCombatActions()) return;

	FVector toTarget = Target->GetActorLocation() - GetControlledMob()->GetActorLocation();

	if (toTarget.Size() > LongRangeAttackDistance * 0.9f)
	{
		UsingLongRangePattern = true;
		CurrentCombatPatternIndex = UKismetMathLibrary::RandomIntegerInRange(0, LongRangeCombatPatterns.Num() - 1);
	}
	else if (toTarget.Size() < MeleeRangeAttackDistance)
	{
		UsingLongRangePattern = false;
		CurrentCombatPatternIndex = UKismetMathLibrary::RandomIntegerInRange(0, MeleeRangeCombatPatterns.Num() - 1);
	}
	else
	{
		CurrentCombatPatternIndex = -1;
	}

}

void AFirewallBossController::OnTargetDeath()
{
	// When player dies, get out of guard mode and stop moving.
	CurrentPhase = 3;
	ControlledFirewall->SwitchOnGuardMode(false);
}