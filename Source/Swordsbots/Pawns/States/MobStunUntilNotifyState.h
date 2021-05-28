// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MobState.h"
#include "CoreMinimal.h"

#include "MobStunUntilNotifyState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStunEndedCallback);

/**
 * Simply disallows all control on the target Swordsbot until a specific Animation Notify is received.
 */
UCLASS()
class SWORDSBOTS_API UMobStunUntilNotifyState : public UMobState
{
	GENERATED_BODY()

public:
	
	ConcurrentSwordsbotControlProfileEnum GetInitialControlProfile() override;
	void SetNotifyName(FName NotifyName);
	void SetStunnedVelocity(FVector Velocity);

	virtual void OnBegin() override;
	virtual void OnUpdate(float DeltaTime) override;
	virtual void OnEnd() override;

	UPROPERTY(BlueprintAssignable)
	FStunEndedCallback StunEndedDelegate;

	UFUNCTION()
	void OnMobAnimationNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

private:

	FName NotifyName;
	FVector Velocity;
};
