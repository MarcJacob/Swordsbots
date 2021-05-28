// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Swordsbot.h"
#include "../../MobState.h"
#include "CoreMinimal.h"
#include "SwordsbotState.generated.h"



/**
 * A Swordsbot State is a time-limited, behavior defining object used to run specific behavior on a Swordsbot, usually
 * with specific data and cached objects with which we want to interact.
 * 
 * States begin on the frame after they are set to begin on the Swordsbot, and end right before another state begins on
 * the target Swordsbot.
 */
UCLASS(Abstract)
class SWORDSBOTS_API USwordsbotState : public UMobState
{
	GENERATED_BODY()

public:

	virtual void AssignControlledMob(AMob_Base* Target) override;

protected:

	FORCEINLINE ASwordsbot* GetTargetSwordsbot() { return TargetSwordsbot; }

private:

	ASwordsbot* TargetSwordsbot;

};
