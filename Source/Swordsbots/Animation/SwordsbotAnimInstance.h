// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobAnimInstance.h"

#include "SwordsbotAnimInstance.generated.h"

/**
 * Specialized Mob Anim Instance able to respond to Swordsbot-specific events on top of regular Mob events.
 */
UCLASS()
class SWORDSBOTS_API USwordsbotAnimInstance : public UMobAnimInstance
{
	GENERATED_BODY()
	
public:
	// Control
	void SetGuardDirection(bool GuardingDown, bool GuardingRight);
	void SetIsInCombatStance(bool IsInCombatStance);

	// Events

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Swordsbot | Combat")
	void OnDash();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Swordsbot | Combat")
	void OnRoll();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Swordsbot")
	void OnEnterCombat();

private:


	// Guard
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Swordsbot | Guard", meta = (AllowPrivateAccess = "true"))
	bool bIsGuardingDown;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Swordsbot | Guard", meta = (AllowPrivateAccess = "true"))
	bool bIsGuardingRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Swordsbot | Guard", meta = (AllowPrivateAccess = "true"))
	bool bIsInCombatStance;

	// Anim Montages

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DashAnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RollAnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Animation | Attacks", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* TopLeftAttackAnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Animation | Attacks", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* TopRightAttackAnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Animation | Attacks", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BottomLeftAttackAnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Animation | Attacks", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BottomRightAttackAnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EnterCombatAnimationMontage;
};
