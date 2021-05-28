// Fill out your copyright notice in the Description page of Project Settings.


#include "MobAIAction.h"
#include "../../../../Pawns/Mob_Base.h"
#include "../MobAIController_Base.h"

void UMobAIAction::AssignMob(AMob_Base* AssignedMob)
{
	ControlledMob = AssignedMob;
	MobController = Cast<AMobAIController_Base>(ControlledMob);
}

void UMobAIAction::Reset_Implementation()
{
	bIsDone = false;
}

void UMobAIAction::OnBegin_Implementation()
{

}

void UMobAIAction::OnTick_Implementation(float DeltaTime)
{

}


