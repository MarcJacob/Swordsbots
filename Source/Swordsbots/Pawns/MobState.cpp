// Fill out your copyright notice in the Description page of Project Settings.


#include "MobState.h"
#include "Mob_Base.h"

void UMobState::BasicInitialization(AMob_Base* Target)
{
	AssignControlledMob(Target);
	ControlProfileBitmask = GetInitialControlProfile();
}

void UMobState::AssignControlledMob(AMob_Base* Target)
{
	TargetMob = Target;
}


void UMobState::EndState()
{
	bEndedExpectedly = true;
	bHasEnded = true;
}