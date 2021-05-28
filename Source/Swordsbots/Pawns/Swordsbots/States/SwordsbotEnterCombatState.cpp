// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordsbotEnterCombatState.h"
#include "../../../Animation/SwordsbotAnimInstance.h"


void USwordsbotEnterCombatState::OnBegin()
{
	GetTargetSwordsbot()->GetSwordsbotAnimation()->OnEnterCombat();
	GetTargetSwordsbot()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &USwordsbotEnterCombatState::OnMobAnimationNotify);
}

void USwordsbotEnterCombatState::OnMobAnimationNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == "EnteredCombat")
	{
		GetTargetSwordsbot()->GetAnimInstance()->OnPlayMontageNotifyBegin.RemoveDynamic(this, &USwordsbotEnterCombatState::OnMobAnimationNotify);
		EndState();
	}
}
