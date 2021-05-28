// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MobAIController_Base.generated.h"

class UMobAIAction;
class AMob_Base;

USTRUCT(BlueprintType)
struct FMobAIPattern
{
	GENERATED_BODY()

	int CurrentActionIndex = -1;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite)
	TArray<UMobAIAction*> Actions;

	void AssignControlledMob(AMob_Base* Mob);
	void UpdatePattern(float DeltaTime);
	bool IsDone();
	void Reset();

};

/**
 * Mob AI Controllers work by using a sort of "sequenced state machine" that follows specific PATTERNS.
 * A PATTERN is a sequence of 'MobAIAction's that drive the controlled entity's behavior, usually for a very short time.
 * The controller itself is a receptacle for different patterns (either single or grouped pattern with random choice)
 * and controls when a specific pattern should start. 
 */
UCLASS(Abstract, Blueprintable)
class SWORDSBOTS_API AMobAIController_Base : public AAIController
{
	GENERATED_BODY()

public:
	void OnPossess(APawn* Possessed) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AMob_Base* GetControlledMob() { return ControlledMob; }

private:
	AMob_Base* ControlledMob;
};
