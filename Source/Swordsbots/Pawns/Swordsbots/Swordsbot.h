// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Mob_Base.h"
#include "../Components/MobCombatComponent.h"
#include "Movement/SwordsbotMovementComponent.h"
#include "Swordsbot.generated.h"

class USwordsbotState;
class USwordsbotRollingState;
class USwordsbotDashingState;

UCLASS()
/// <summary>
/// Advanced Mob class featuring the ability of dynamically switching between all four attack & defense directions,
/// dash and roll aswell as sprint and switch between Normal and Combat stance.
/// </summary>
class SWORDSBOTS_API ASwordsbot : public AMob_Base
{
	GENERATED_BODY()

public:
	ASwordsbot(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void MoveTowards(FVector Direction) override;

	// Swordsbot specific Actions

	UFUNCTION(BlueprintCallable)
	USwordsbotDashingState* Dash(FVector2D Direction);
	UFUNCTION(BlueprintCallable)
	USwordsbotRollingState* Roll(FVector2D Direction);
	UFUNCTION(BlueprintCallable)
	void SetIsSprinting(bool Sprinting);
	UFUNCTION(BlueprintCallable)
	void SetIsInCombatStance(bool CombatStance);
	UFUNCTION(BlueprintCallable)
	void SetGuard(bool GuardingDown, bool GuardingRight);

	// Swordsbot State
	FORCEINLINE bool IsInCombatStance() { return bIsInCombatStance; }
	
	FORCEINLINE class USwordsbotAnimInstance* GetSwordsbotAnimation() { return Cast<USwordsbotAnimInstance>(GetAnimInstance()); }
	FORCEINLINE USwordsbotMovementComponent* GetSwordsbotMovementComponent() { return Cast<USwordsbotMovementComponent>(GetMovementComponent()); }

protected:

	FORCEINLINE const USwordsbotState* GetCurrentSwordsbotState() { return Cast<USwordsbotState>(GetCurrentState()); }

	// PROPERTIES >

	// DASHING
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Combat", meta = (AllowPrivateAccess = "true"))
	float DashTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Combat", meta = (AllowPrivateAccess = "true"))
	float DashDistance;

	// ROLLING

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Combat", meta = (AllowPrivateAccess = "true"))
	float RollTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Combat", meta = (AllowPrivateAccess = "true"))
	float RollDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Combat", meta = (AllowPrivateAccess = "true"))
	float RollInvulnerabilityTime;

	// COMBAT
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Combat", meta = (AllowPrivateAccess = "true"))
	float CombatStanceMovementSpeedModifier;

	// < PROPERTIES

private:
	// DEBUG
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	bool bPrintStateChangeToScreen;

	// State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Swordsbot | State", meta = (AllowPrivateAccess = "true"))
	bool bIsInCombatStance;

};
