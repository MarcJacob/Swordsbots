// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordsbotPlayerController.h"
#include "../../Pawns/Swordsbots/Swordsbot.h"
#include "../../Map/InteractableMapElement.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/PawnMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include "../../GameInstance/PlaythroughGameSubsystem.h"

void ASwordsbotPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ControlledSwordsbot->IsInCombatStance() != bCombatStanceDesired)
	{
		ControlledSwordsbot->SetIsInCombatStance(bCombatStanceDesired);
		if (bCombatStanceDesired == false) // We just switched to normal stance
		{
			ControlledSwordsbot->UnlockRotation();
		}
	}

	if (ControlledSwordsbot->IsInCombatStance())
	{
		bool guardingDown, guardingRight;

		if (UKismetMathLibrary::Abs(LastMouseInput.X) > GuardSwitchMinMovement.X)
		{
			LastAppliedGuardInput.X = LastMouseInput.X;
			guardingRight = LastMouseInput.X > 0;
		}
		else
		{
			guardingRight = LastAppliedGuardInput.X > 0;
		}

		if (UKismetMathLibrary::Abs(LastMouseInput.Y) > GuardSwitchMinMovement.Y)
		{
			LastAppliedGuardInput.Y = LastMouseInput.Y;
			guardingDown = LastMouseInput.Y > 0;
		}
		else
		{
			guardingDown = LastAppliedGuardInput.Y > 0;
		}

		ControlledSwordsbot->SetGuard(guardingDown, guardingRight);

		// Update locked rotation
		ControlledSwordsbot->LockRotationTo(GetControlRotation());
	}
	
	UCameraComponent* camera = Cast<UCameraComponent>(ControlledSwordsbot->GetComponentByClass(UCameraComponent::StaticClass()));
	
	// Interactable detection

	bool resetUsableMapElement = true;

	// World Static interactables pass
	FCollisionObjectQueryParams objectQueryParams = FCollisionObjectQueryParams(ECC_WorldStatic);
	FHitResult result;
	if (GetWorld()->LineTraceSingleByObjectType(result, camera->GetComponentLocation(), camera->GetComponentLocation() + camera->GetForwardVector() * 800.f, objectQueryParams))
	{
		AInteractableMapElement* interactable = Cast<AInteractableMapElement>(result.Actor);
		if (interactable != nullptr)
		{
			resetUsableMapElement = false;
			CurrentUsableMapElement = interactable;
			CurrentInteractionRay = result;
		}
	}

	// World Dynamic interactables pass
	objectQueryParams = FCollisionObjectQueryParams(ECC_WorldDynamic);
	if (GetWorld()->LineTraceSingleByObjectType(result, camera->GetComponentLocation(), camera->GetComponentLocation() + camera->GetForwardVector() * 800.f, objectQueryParams))
	{
		AInteractableMapElement* interactable = Cast<AInteractableMapElement>(result.Actor);
		if (interactable != nullptr)
		{
			resetUsableMapElement = false;
			CurrentUsableMapElement = interactable;
			CurrentInteractionRay = result;
		}
	}

	if (resetUsableMapElement)
	{
		CurrentUsableMapElement = nullptr;
	}
}

void ASwordsbotPlayerController::OnPossess(APawn* possessedPawn)
{
	Super::OnPossess(possessedPawn);
	ControlledSwordsbot = Cast<ASwordsbot>(possessedPawn);
	ControlledSwordsbot->OnDeathDelegate.AddDynamic(this, &ASwordsbotPlayerController::ControlledSwordsbotDied);
	if (ControlledSwordsbot == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Error : SwordsbotPlayerController can only possess a Swordsbot Pawn !"));
		return;
	}
}

void ASwordsbotPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ASwordsbotPlayerController::ControlledSwordsbotDied_Implementation()
{
	GetGameInstance()->GetSubsystem<UPlaythroughGameSubsystem>()->SignalPlayerDied();
}

void ASwordsbotPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis(TEXT("Movement_Vertical"), this, &ASwordsbotPlayerController::VerticalMovement);
	InputComponent->BindAxis(TEXT("Movement_Horizontal"), this, &ASwordsbotPlayerController::HorizontalMovement);

	InputComponent->BindAxis(TEXT("Mouse_X"), this, &ASwordsbotPlayerController::OnMouseX);
	InputComponent->BindAxis(TEXT("Mouse_Y"), this, &ASwordsbotPlayerController::OnMouseY);

	InputComponent->BindAction(TEXT("Jump/Dash"), IE_Pressed, this, &ASwordsbotPlayerController::JumpDash);
	
	InputComponent->BindAction(TEXT("CombatStance"), IE_Pressed, this, &ASwordsbotPlayerController::SwitchOnCombatStance);
	InputComponent->BindAction(TEXT("CombatStance"), IE_Released, this, &ASwordsbotPlayerController::SwitchOffCombatStance);

	InputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &ASwordsbotPlayerController::StartAttack);

	InputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ASwordsbotPlayerController::Interact);

	InputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ASwordsbotPlayerController::StartSprinting);
	InputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ASwordsbotPlayerController::StopSprinting);
}

void ASwordsbotPlayerController::VerticalMovement(float normalizedVerticalAxis)
{
	if (UKismetMathLibrary::NearlyEqual_FloatFloat(normalizedVerticalAxis, 0.f))
	{
		return;
	}
	FVector direction = GetControlRotation().Quaternion() * FVector::ForwardVector;
	direction.Z = 0.f;
	direction.Normalize();
	ControlledSwordsbot->MoveTowards(direction * normalizedVerticalAxis);
}

void ASwordsbotPlayerController::HorizontalMovement(float normalizedHorizontalAxis)
{
	FVector direction = GetControlRotation().Quaternion() * FVector::RightVector;
	direction.Z = 0.f;
	ControlledSwordsbot->MoveTowards(direction * normalizedHorizontalAxis);
}

void ASwordsbotPlayerController::OnMouseX(float normalizedMouseX)
{
	AddYawInput(normalizedMouseX);
	LastMouseInput.X = normalizedMouseX;
}

void ASwordsbotPlayerController::OnMouseY(float normalizedMouseY)
{
	AddPitchInput(normalizedMouseY);
	LastMouseInput.Y = normalizedMouseY;
}

void ASwordsbotPlayerController::JumpDash()
{
	if (ControlledSwordsbot->IsInCombatStance())
	{
		FVector dashDirection = ControlledSwordsbot->GetMovementComponent()->GetLastInputVector();
		if (dashDirection.IsNearlyZero())
		{
			dashDirection = GetControlRotation().Vector();
			dashDirection.Normalize();
		}
		if (bSprintMode) ControlledSwordsbot->Roll(FVector2D(dashDirection.X, dashDirection.Y));
		else ControlledSwordsbot->Dash(FVector2D(dashDirection.X, dashDirection.Y));
	}
	else
	{
		ControlledSwordsbot->Jump();
	}
}

void ASwordsbotPlayerController::SwitchOnCombatStance()
{
	bCombatStanceDesired = true;
}

void ASwordsbotPlayerController::SwitchOffCombatStance()
{
	bCombatStanceDesired = false;
}

void ASwordsbotPlayerController::StartAttack()
{
	ControlledSwordsbot->Attack();
}

void ASwordsbotPlayerController::Interact()
{
	if (CurrentUsableMapElement != nullptr && CurrentUsableMapElement->bCanInteract)
	{
		CurrentUsableMapElement->OnInteraction(this, GetPawn(), CurrentInteractionRay);
	}
}

void ASwordsbotPlayerController::StartSprinting()
{
	ControlledSwordsbot->SetIsSprinting(true);
	bSprintMode = true;
}

void ASwordsbotPlayerController::StopSprinting()
{
	ControlledSwordsbot->SetIsSprinting(false);
	bSprintMode = false;
}
