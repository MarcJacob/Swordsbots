// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../MobState.h"
#include "Curves/CurveFloat.h"
#include "FirewallJumpAttackState.generated.h"

/**
 * 
 */
UCLASS()
class SWORDSBOTS_API UFirewallJumpAttackState : public UMobState
{
	GENERATED_BODY()
	
public:

	virtual ConcurrentSwordsbotControlProfileEnum GetInitialControlProfile() override
	{
		return DISALLOWS_ALL;
	}

	UFUNCTION(BlueprintCallable)
	void SetJumpCurveData(AActor* TargetActor, float Height, float TravelTime, UCurveFloat* HeightCurve);
	UFUNCTION(BlueprintCallable)
	void SetJumpCurveDataWithPosition(FVector TargetPosition, float Height, float TravelTime, UCurveFloat* HeightCurve);

	UFUNCTION(BlueprintCallable)
	void SetAttackDamage(int Damage);

	UFUNCTION(BlueprintCallable)
	void SetLookAtTargetPosition(bool LookAtTarget);
	
	void OnBegin() override;
	void OnUpdate(float DeltaTime) override;
	void OnEnd() override;

	UFUNCTION()
	void OnAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

private:

	float StartHeight;
	float JumpHeight;
	bool bLookAtTargetPosition = true;

	AActor* Target;
	FVector FinalJumpTargetPosition;
	FVector JumpStartPosition;

	float CurveTravelRatio;
	float CurveTravelTime;

	UCurveFloat* HeightRatioCurve;

	int AttackDamage;

	bool bTravellingOnCurve = false;
};
