// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordsbotFighterMobAI.h"
#include "../../../Pawns/Swordsbots/Swordsbot.h"


void ASwordsbotFighterMobAI::OnPossess(APawn* Possessed)
{
	Super::OnPossess(Possessed);

	ControlledSwordsbot = Cast<ASwordsbot>(Possessed);
	if (ControlledSwordsbot == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR : Attempted to possess a non-Swordsbot Pawn with a Swordsbot AI controller."));
		Destroy();
	}
}

void ASwordsbotFighterMobAI::OutOfCombatTick(float DeltaTime)
{
	Super::OutOfCombatTick(DeltaTime);

	if (ControlledSwordsbot->IsInCombatStance())
	{
		ControlledSwordsbot->SetIsInCombatStance(false);
	}
}

void ASwordsbotFighterMobAI::MovementToTargetTick(float DeltaTime)
{
	Super::MovementToTargetTick(DeltaTime);

	FVector toTarget = GetTarget()->GetActorLocation() - ControlledSwordsbot->GetActorLocation();
	float distToTarget = toTarget.Size();
	if (distToTarget > GuardDistanceFromTarget && ControlledSwordsbot->IsInCombatStance())
	{
		ControlledSwordsbot->SetIsInCombatStance(false);
	}
	else if (distToTarget < GuardDistanceFromTarget && !ControlledSwordsbot->IsInCombatStance())
	{
		ControlledSwordsbot->SetIsInCombatStance(true);
	}

}

void ASwordsbotFighterMobAI::InCombatRangeTick(float DeltaTime)
{
	FVector toTarget = GetTarget()->GetActorLocation() - ControlledSwordsbot->GetActorLocation();
	SetControlRotation(toTarget.Rotation());
	Super::InCombatRangeTick(DeltaTime);
}
