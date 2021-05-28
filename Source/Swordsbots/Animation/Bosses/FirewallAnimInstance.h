// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MobAnimInstance.h"
#include "FirewallAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SWORDSBOTS_API UFirewallAnimInstance : public UMobAnimInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void OnKneel();
	UFUNCTION(BlueprintImplementableEvent)
	void OnStandup();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnJumpAttack();
	UFUNCTION(BlueprintImplementableEvent)
	void OnBackflip();

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchGuardMode(bool GuardModeEnabled);
};
