// Fill out your copyright notice in the Description page of Project Settings.


#include "MobAIController_Base.h"
#include "../../../Pawns/Mob_Base.h"
#include "Actions/MobAIAction.h"


void FMobAIPattern::AssignControlledMob(AMob_Base* Mob)
{
	for (UMobAIAction* action : Actions)
	{
		action->AssignMob(Mob);
	}
}

void FMobAIPattern::Reset()
{
	CurrentActionIndex = -1;
	for (UMobAIAction* action : Actions)
	{
		action->Reset();
	}
}

void FMobAIPattern::UpdatePattern(float DeltaTime)
{
	if (CurrentActionIndex < 0)
	{
		CurrentActionIndex = 0;
		Actions[0]->OnBegin();
	}

	if (CurrentActionIndex < Actions.Num())
	{
		
		if (Actions[CurrentActionIndex]->IsDone())
		{
			CurrentActionIndex++;
			if (CurrentActionIndex < Actions.Num())
			Actions[CurrentActionIndex]->OnBegin();
		}
		else
		{
			Actions[CurrentActionIndex]->OnTick(DeltaTime);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WARNING : Attempted to update a finished pattern !"));
	}
}

bool FMobAIPattern::IsDone()
{
	return CurrentActionIndex >= Actions.Num();
}

void AMobAIController_Base::OnPossess(APawn* Possessed)
{
	Super::OnPossess(Possessed);

	// Check possessed Pawn to make sure it inherits from AMob_Base and set the ControlledMob member variable.
	AMob_Base* possessedToMob = Cast<AMob_Base>(Possessed);
	if (possessedToMob)
	{
		ControlledMob = possessedToMob;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR : Cannot assign Mob AI Controller to a non Mob_Base-inheriting Pawn !"));
		Destroy();
	}
}
