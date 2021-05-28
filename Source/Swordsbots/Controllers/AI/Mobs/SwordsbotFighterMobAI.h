// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleFighterMobAI.h"
#include "SwordsbotFighterMobAI.generated.h"

class ASwordsbot;

/**
 * More advanced fighter Mob AI that assumes it's controlling a Swordsbot - able to switch between different guards,
 * block & parry hits, dash, roll...
 */
UCLASS()
class SWORDSBOTS_API ASwordsbotFighterMobAI : public ASimpleFighterMobAI
{
	GENERATED_BODY()
	
public:

	void OnPossess(APawn* Possessed) override;

protected:

	virtual void OutOfCombatTick(float DeltaTime) override;
	virtual void MovementToTargetTick(float DeltaTime) override;
	virtual void InCombatRangeTick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float GuardDistanceFromTarget;

	ASwordsbot* ControlledSwordsbot;
};
