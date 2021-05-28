// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MobState.generated.h"

class AMob_Base;

/// <summary>
/// By default, a state does not in any way prevent normal control of the Target Swordsbot. With the control profile,
/// a state can effectively take over part of or the entirety of the Swordsbot's behavior.
/// </summary>
enum ConcurrentSwordsbotControlProfileEnum : uint8
{
	NO_CONSTRAINTS = 0,


	OVERRIDES_DEFAULT_BEHAVIOR = 1,
	DISALLOWS_MOVEMENT = 1 << 1,
	DISALLOWS_JUMP = 1 << 2,
	DISALLOWS_DASH = 1 << 3,
	DISALLOWS_COMBAT_ACTIONS = 1 << 4,
	DISALLOWS_STANCE_SWITCH = 1 << 5,

	DISALLOWS_ALL = OVERRIDES_DEFAULT_BEHAVIOR | DISALLOWS_MOVEMENT | DISALLOWS_JUMP | DISALLOWS_DASH
	| DISALLOWS_COMBAT_ACTIONS | DISALLOWS_STANCE_SWITCH,
};

/**
 * A Mob State is a direct behavior driver for a Mob, over a certain period of time.
 * States assume they have complete or almost complete control of their target Mob.
 * 
 * States are not made to drive intelligent behavior but rather represent a single Action.
 */
UCLASS(Abstract, BlueprintType)
class SWORDSBOTS_API UMobState : public UObject
{
	GENERATED_BODY()

public:

	void BasicInitialization(AMob_Base* Target);
	virtual void AssignControlledMob(AMob_Base* Target);

	virtual void OnBegin() {}
	virtual void OnUpdate(float DeltaTime) {}
	virtual void OnEnd() { bHasEnded = true; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool HasEnded() const { return bHasEnded; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool HasEndedExpectedly() const { return bEndedExpectedly; }

	FORCEINLINE AMob_Base* GetControlledMob() { return TargetMob; }

protected:

	/// <summary>
	/// Triggers the current state to end and sets "EndedExpectedly" to true.
	/// </summary>
	void EndState();

private:

	AMob_Base* TargetMob;
	bool bHasEnded;
	bool bEndedExpectedly; // False until the state ends itself by calling "EndState"

// State Mob Control permissions
public:

	virtual ConcurrentSwordsbotControlProfileEnum GetInitialControlProfile() { return NO_CONSTRAINTS; }

	FORCEINLINE bool OverridesDefaultBehavior() const { return !HasEnded() && ControlProfileBitmask & OVERRIDES_DEFAULT_BEHAVIOR; }
	FORCEINLINE bool DisallowsMovement() const { return !HasEnded() && ControlProfileBitmask & DISALLOWS_MOVEMENT; }
	FORCEINLINE bool DisallowsDashing() const { return !HasEnded() && ControlProfileBitmask & DISALLOWS_DASH; }
	FORCEINLINE bool DisallowsCombatActions() const { return !HasEnded() && ControlProfileBitmask & DISALLOWS_COMBAT_ACTIONS; }
	FORCEINLINE bool DisallowsStanceSwitch() const { return !HasEnded() && ControlProfileBitmask & DISALLOWS_STANCE_SWITCH; }

private:
	ConcurrentSwordsbotControlProfileEnum ControlProfileBitmask;
};
