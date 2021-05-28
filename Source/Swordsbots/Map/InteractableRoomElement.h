// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableMapElement.h"
#include "InteractableRoomElement.generated.h"

class ARoomLevel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRoomAssignedCallback, ARoomLevel*, AssignedRoom);

/**
 * An interactable Map Element that requires a link to a specific room to function.
 */
UCLASS()
class SWORDSBOTS_API AInteractableRoomElement : public AInteractableMapElement
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
		void AssignRoom(ARoomLevel* RoomLevel);

	UPROPERTY(BlueprintAssignable)
		FRoomAssignedCallback RoomAssignedDelegate;

	FORCEINLINE ARoomLevel* GetAssignedRoom() { return AssignedRoom; }

private:

	ARoomLevel* AssignedRoom;
	
};
