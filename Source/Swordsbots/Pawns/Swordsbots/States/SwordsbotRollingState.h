// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwordsbotState.h"
#include "SwordsbotRollingState.generated.h"

/**
 * 
 */
UCLASS()
class SWORDSBOTS_API USwordsbotRollingState : public USwordsbotState
{
	GENERATED_BODY()
	
public:

	ConcurrentSwordsbotControlProfileEnum GetInitialControlProfile() override;
	void SetRollProperties(float RollSpeed, float RollTime, FVector RollDirection, float InvulnerableTime);

	virtual void OnBegin() override;
	virtual void OnUpdate(float DeltaTime) override;

private:

	float SpeedPerSecond;
	float TotalTime;
	float ElapsedTime;
	FVector Direction;
	float InvulnerableTime;
};
