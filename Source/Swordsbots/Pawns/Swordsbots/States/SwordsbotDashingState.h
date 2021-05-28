// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SwordsbotState.h"
#include "CoreMinimal.h"

#include "SwordsbotDashingState.generated.h"

/**
 * 
 */
UCLASS()
class SWORDSBOTS_API USwordsbotDashingState : public USwordsbotState
{
	GENERATED_BODY()

public:

	ConcurrentSwordsbotControlProfileEnum GetInitialControlProfile() override;
	void SetDashProperties(float DashSpeed, float DashTime, FVector DashDirection);

	virtual void OnBegin() override;
	virtual void OnUpdate(float DeltaTime) override;
	virtual void OnEnd() override;

private:

	float SpeedPerSecond;
	float TotalTime;
	float ElapsedTime;
	FVector Direction;
};
