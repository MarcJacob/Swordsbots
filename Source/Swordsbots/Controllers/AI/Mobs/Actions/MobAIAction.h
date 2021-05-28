// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MobAIAction.generated.h"

class AMob_Base;
class AMobAIController_Base;

/**
 * Behavior-driver for a mob, usually for a very short time. Represents a single "action", often within a pattern.
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class SWORDSBOTS_API UMobAIAction : public UObject
{
	GENERATED_BODY()
	
public:

	// TODO : Replace Pawn with a lower level type (like AMob)
	virtual void AssignMob(AMob_Base* AssignedMob);
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsDone() { return bIsDone; }

	UFUNCTION(BlueprintNativeEvent)
	void Reset();
	UFUNCTION(BlueprintNativeEvent)
	void OnBegin();
	UFUNCTION(BlueprintNativeEvent)
	void OnTick(float DeltaTime);

protected:

	UFUNCTION(BlueprintCallable)
	void EndAction() { bIsDone = true; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AMob_Base* GetControlledMob() { return ControlledMob; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AMobAIController_Base* GetMobController() { return MobController; }

private:

	AMob_Base* ControlledMob;
	AMobAIController_Base* MobController;
	bool bIsDone;
};
