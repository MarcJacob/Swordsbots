// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MobState.h"
#include "CoreMinimal.h"

#include "MobDeadState.generated.h"

/**
 * 
 */
UCLASS()
class SWORDSBOTS_API UMobDeadState : public UMobState
{
	GENERATED_BODY()

public:

	void SetDespawnTime(float DespawnTime);
	void SetTurnToRagdoll(bool TurnToRagdoll);

	virtual ConcurrentSwordsbotControlProfileEnum GetInitialControlProfile() override;

	virtual void OnBegin() override;
	virtual void OnUpdate(float DeltaTime) override;

	UFUNCTION()
	void OnAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

private:

	float TimeUntilDespawn;
	bool bTurnToRagdoll = true;
};
