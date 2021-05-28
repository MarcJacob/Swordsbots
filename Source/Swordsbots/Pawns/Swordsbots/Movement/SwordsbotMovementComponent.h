// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Components/MobMovementComponent.h"
#include "SwordsbotMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SWORDSBOTS_API USwordsbotMovementComponent : public UMobMovementComponent
{
	GENERATED_BODY()
	
public:

	FORCEINLINE void StartSprinting() { bSprintDesired = true; }
	FORCEINLINE void StopSprinting() { bSprintDesired = false; }

protected:

	virtual void GroundedStateTick(float DeltaTime) override;

	void AirControlsStateTick(float DeltaTime);

private:

	bool bSprintDesired;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swordsbot Movement | Normal Mode", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swordsbot Movement | Guard Mode", meta = (AllowPrivateAccess = "true"))
	float ForwardStepSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swordsbot Movement | Guard Mode", meta = (AllowPrivateAccess = "true"))
	float StrafeSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swordsbot Movement | HSN Mode", meta = (AllowPrivateAccess = "true"))
	float HSNForwardAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swordsbot Movement | HSN Mode", meta = (AllowPrivateAccess = "true"))
	float HSNJumpForce;
};
