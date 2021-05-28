// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableRoomElement.h"


void AInteractableRoomElement::AssignRoom(ARoomLevel* RoomLevel)
{
	if (AssignedRoom == nullptr)
	{
		AssignedRoom = RoomLevel;
		RoomAssignedDelegate.Broadcast(RoomLevel);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR : Attempted to assign a Room to a Interactable Room Element while it already had one assigned."))
	}
}