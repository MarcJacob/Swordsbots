// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobAIController_Base.h"
#include "SimpleFighterMobAI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFighterMobAITargetChangeCallback, AActor*, OldTarget, AActor*, NewTarget);

/**
 * Attempts to stay at a certain distance from the target, at which point it will choose between one of its
 * Attack Patterns. Between two executions of the pattern, it will execute the Waiting Pattern.
 */
UCLASS(Blueprintable)
class SWORDSBOTS_API ASimpleFighterMobAI : public AMobAIController_Base
{
	GENERATED_BODY()
	
public:

	void OnPossess(APawn* Possessed) override;
	void Tick(float DeltaTime) override;

	FORCEINLINE const AActor* GetTarget() const { return Target; }
	FORCEINLINE bool IsInCombat() { return Target != nullptr && IsValid(Target); }
	FORCEINLINE float GetPreferedCombatDistance() { return CombatDistance; }

	UPROPERTY(BlueprintAssignable)
	FFighterMobAITargetChangeCallback TargetChangedDelegate;

protected:

	void ChangeTarget(AActor* NewTarget);

	virtual void OutOfCombatTick(float DeltaTime);
	virtual void MovementToTargetTick(float DeltaTime);
	virtual void InCombatRangeTick(float DeltaTime);

	FORCEINLINE bool IsInCombatRange() const { return bIsInCombatRange; }

private:

	// Configuration properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float CombatDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float TimeBetweenCombatPatterns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TArray<FMobAIPattern> CombatPatterns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FMobAIPattern WaitingPattern;

	// State

	AActor* Target;
	bool bIsInCombatRange;
	int CurrentCombatPatternIndex;
	float TimeUntilNextCombatPattern;
};
