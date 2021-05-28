// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../../Swordsbots/Pawns/Components/MobCombatComponent.h"
#include "MobAnimInstance.generated.h"

/**
 * Anim Instance able to react to common Mob events such as moving around, attacking, taking damage / dying...
 */
UCLASS()
class SWORDSBOTS_API UMobAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	void UpdateSpeedParameters(float NormalizedForwardSpeed, float NormalizedSideSpeed);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Mob | Movement")
	void OnJumpStarted();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Mob | Movement")
	void OnLanded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Mob | Combat")
	void OnHurtFront();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Mob | Combat")
	void OnDeath();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Mob | Combat")
	void OnAttack(EGuardDirection AttackDirection);

	UFUNCTION(BlueprintCallable, Category = "Mob | Combat")
	void OnAttackWithSpecificAnimation(UAnimMontage* AnimationMontage);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Mob | Combat")
	void OnBlocked();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Mob | Combat")
	void OnParried();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Mob | Combat")
	void OnBlocking();

private:

	// Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob | Movement", meta = (AllowPrivateAccess = "true"))
	float NormalizedForwardSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob | Movement", meta = (AllowPrivateAccess = "true"))
	float NormalizedSideSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mob | Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathAnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mob | Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MainAttackAnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mob | Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* BlockedAnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mob | Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ParriedAnimationMontage;
};
