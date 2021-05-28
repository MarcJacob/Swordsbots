// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordsbotAnimInstance.h"
#include "../Pawns/Swordsbots/Swordsbot.h"

void USwordsbotAnimInstance::SetGuardDirection(bool GuardingDown, bool GuardingRight)
{
	this->bIsGuardingDown = GuardingDown;
	this->bIsGuardingRight = GuardingRight;
}

void USwordsbotAnimInstance::SetIsInCombatStance(bool IsInCombatStance)
{
	this->bIsInCombatStance = IsInCombatStance;
}