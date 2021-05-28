// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Swordsbot.h"
#include "PlayerSwordsbot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSwordsbotMaxHealthChanged, int, Delta);

class ASwordsbotPlayerController;

/**
 * 
 */
UCLASS()
class SWORDSBOTS_API APlayerSwordsbot : public ASwordsbot
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void ChangeMaxHealth(int MaxHealthDelta);
	UPROPERTY(BlueprintAssignable)
	FOnPlayerSwordsbotMaxHealthChanged MaxHealthChangedDelegate;

	// Shadowing declaration
	ASwordsbotPlayerController* GetController();
};
