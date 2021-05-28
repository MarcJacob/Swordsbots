// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableRoomElement.h"
#include "RoomActivator.generated.h"



/**
 * 
 */
UCLASS()
class SWORDSBOTS_API ARoomActivator : public AInteractableRoomElement
{
	GENERATED_BODY()
	
public:

	virtual void OnInteraction_Implementation(AController* UserController, AActor* Controller, const FHitResult& UserToInteractable) override;
};
