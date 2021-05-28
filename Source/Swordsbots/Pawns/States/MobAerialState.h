// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MobState.h"
#include "CoreMinimal.h"

#include "MobAerialState.generated.h"

/**
 * 
 */
UCLASS()
class SWORDSBOTS_API UMobAerialState : public UMobState
{
	GENERATED_BODY()

public:
	virtual ConcurrentSwordsbotControlProfileEnum GetInitialControlProfile() override;

	virtual void OnBegin() override;
	virtual void OnUpdate(float DeltaTime) override;

private:
	void OnControlledMobLanded(const FHitResult& LandingHit);
};
