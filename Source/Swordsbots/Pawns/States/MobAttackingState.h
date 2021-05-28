// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MobState.h"
#include "../Components/MobCombatComponent.h"
#include "CoreMinimal.h"

#include "MobAttackingState.generated.h"

/**
 * 
 */
UCLASS()
class SWORDSBOTS_API UMobAttackingState : public UMobState
{
	GENERATED_BODY()
	
public:
	
	ConcurrentSwordsbotControlProfileEnum GetInitialControlProfile() override;

	UFUNCTION(BlueprintCallable)
	void SetAttackDirection(EGuardDirection AttackDirection) { Direction = AttackDirection; }
	UFUNCTION(BlueprintCallable)
	void SetSpecificAnimation(UAnimMontage* AnimationToUse) { SpecificAnimationMontage = AnimationToUse; }
	UFUNCTION(BlueprintCallable)
	void SetDamage(int Damage) { DamageAmount = Damage; }
	UFUNCTION(BlueprintCallable)
	void SetPossibleDefense(bool CanBlock, bool CanParry) { bIsBlockable = CanBlock; bIsParriable = CanParry; }

	virtual void OnBegin() override;
	virtual void OnUpdate(float DeltaTime) override;
	virtual void OnEnd() override;

private:

	UFUNCTION()
	void OnMobAnimationNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	void OnHitNotify();
	void OnActionEndNotify();
	void OnBlocked();
	void OnParried();

	EGuardDirection Direction;
	UAnimMontage* SpecificAnimationMontage = nullptr;
	int DamageAmount = 1;
	bool bIsBlockable = true;
	bool bIsParriable = true;
};
