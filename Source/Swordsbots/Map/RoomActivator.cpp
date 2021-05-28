// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomActivator.h"
#include "RoomLevel.h"

void ARoomActivator::OnInteraction_Implementation(AController* UserController, AActor* User, const FHitResult& UserToInteractable)
{
	if (!bCanInteract) return;
	if (GetAssignedRoom() != nullptr)
	{
		// Move the User inside the room and activate it.
		User->SetActorLocation(User->GetActorLocation() - UserToInteractable.ImpactNormal * 300.f);
		GetAssignedRoom()->OnRoomActivated();
		bCanInteract = false;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR : Attempted to activate a Room Activator without it having a Room assigned."))
	}
}

