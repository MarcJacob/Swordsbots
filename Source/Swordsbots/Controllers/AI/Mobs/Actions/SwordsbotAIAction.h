// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobAIAction.h"
#include "SwordsbotAIAction.generated.h"

class ASwordsbot;

/**
 * Mob AI Action which checks that the controlled Actor is of type (or a type derived from) Swordsbot.
 * This allows the making of Swordsbot specific AI Actions that have access to Swordsbot specific capabilities like
 * rolling, dashing...
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class SWORDSBOTS_API USwordsbotAIAction : public UMobAIAction
{
	GENERATED_BODY()

public:

	virtual void AssignMob(AMob_Base* AssignedMob) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ASwordsbot* GetControlledSwordsbot() { return ControlledSwordsbot; }

private:

	ASwordsbot* ControlledSwordsbot;
	
};
