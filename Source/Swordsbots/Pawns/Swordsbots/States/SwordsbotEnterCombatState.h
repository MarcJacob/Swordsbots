// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwordsbotState.h"
#include "SwordsbotEnterCombatState.generated.h"

/**
 * 
 */
UCLASS()
class SWORDSBOTS_API USwordsbotEnterCombatState : public USwordsbotState
{
	GENERATED_BODY()

public:

	virtual ConcurrentSwordsbotControlProfileEnum GetInitialControlProfile() override { return DISALLOWS_ALL; }
	
	virtual void OnBegin() override;

	UFUNCTION()
	void OnMobAnimationNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

};
