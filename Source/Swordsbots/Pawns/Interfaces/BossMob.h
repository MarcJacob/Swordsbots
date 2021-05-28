// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RoomMob.h"
#include "BossMob.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UBossMob : public URoomMob
{
	GENERATED_BODY()
};

/**
 *
 */
class SWORDSBOTS_API IBossMob : public IRoomMob
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable)
	virtual void StartNextPhase() = 0;
};
