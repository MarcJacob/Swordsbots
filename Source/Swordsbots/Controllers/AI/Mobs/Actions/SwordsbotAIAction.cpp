// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordsbotAIAction.h"
#include "../../../../Pawns/Swordsbots/Swordsbot.h"

void USwordsbotAIAction::AssignMob(AMob_Base* AssignedMob)
{
	Super::AssignMob(AssignedMob);
	ASwordsbot* swordsbot = Cast<ASwordsbot>(AssignedMob);

	if (swordsbot != nullptr)
	{
		ControlledSwordsbot = swordsbot;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR : Attempted to use a Swordsbot AI Action on a non-Swordsbot Mob."));
	}
}