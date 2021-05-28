// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SwordsbotPlayerController.generated.h"

class AInteractableMapElement;
class ASwordsbot;

/**
 * Player Controller dedicated to controlling a Swordsbot Pawn.
 */
UCLASS()
class SWORDSBOTS_API ASwordsbotPlayerController : public APlayerController
{
	GENERATED_BODY()

		// Public interfacing functions
public:

	virtual void OnPossess(APawn* possessedPawn) override;
	virtual void OnUnPossess() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintNativeEvent)
	void ControlledSwordsbotDied();

	FORCEINLINE
	ASwordsbot* GetControlledSwordsbot() { return ControlledSwordsbot; }

private:

	// Config properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Control | Combat", meta = (AllowPrivateAccess = "true"))
	FVector2D GuardSwitchMinMovement;

	// Internal controller functions
protected:
	void VerticalMovement(float normalizedVerticalAxis);
	void HorizontalMovement(float normalizedHorizontalAxis);
	void OnMouseX(float normalizedMouseX);
	void OnMouseY(float normalizedMouseY);
	void JumpDash();
	void SwitchOnCombatStance();
	void SwitchOffCombatStance();
	void StartAttack();
	void Interact();
	void StartSprinting();
	void StopSprinting();

private:

	bool bCombatStanceDesired;
	bool bSprintMode;
	ASwordsbot* ControlledSwordsbot;
	FVector2D LastMouseInput;
	FVector2D LastAppliedGuardInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	AInteractableMapElement* CurrentUsableMapElement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	FHitResult CurrentInteractionRay;
};
