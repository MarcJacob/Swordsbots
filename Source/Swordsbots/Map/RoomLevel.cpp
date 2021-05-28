// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomLevel.h"
#include "../Pawns/Swordsbots/Swordsbot.h"
#include "../GameInstance/PlaythroughGameSubsystem.h"
#include "../DataObjects/LootObject.h"



ARoomLevel::ARoomLevel()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARoomLevel::OnRoomSpawned_Implementation()
{
	TArray<AActor*> levelActors = GetLevel()->Actors;

	for (AActor* actor : levelActors)
	{
		IRoomMob* mobPtr = Cast<IRoomMob>(actor);
		if (mobPtr != nullptr)
		{
			RoomMobs.Add(mobPtr);
		}
	}

	for (IRoomMob* mob : RoomMobs)
	{
		APawn* p = Cast<APawn>(mob);
		if (p != nullptr)
		{
			p->AutoPossessAI = EAutoPossessAI::Disabled;
		}
		
	}
	bActivated = false;
	OnSpawnedDelegate.Broadcast();
}

void ARoomLevel::OnRoomActivated_Implementation()
{

	if (RoomMobs.Num() > 0)
	for (IRoomMob* mob : RoomMobs)
	{
		APawn* pawn = Cast<APawn>(mob);
		AController* pawnController = GetWorld()->SpawnActor<AController>(pawn->AIControllerClass);
		if (pawnController != nullptr)
		{
			pawnController->Possess(pawn);
		}

		
	}

	bActivated = true;
	OnActivatedDelegate.Broadcast();
}

void ARoomLevel::OnRoomCleared_Implementation()
{
	OnClearedDelegate.Broadcast();
	GetWorld()->GetGameInstance()->GetSubsystem<UPlaythroughGameSubsystem>()->SignalRoomCleared(this);
}

void ARoomLevel::Tick(float DeltaTime)
{
	if (bActivated && !bRoomCleared)
	{
		bool cleared = true;
		for (IRoomMob* mob : RoomMobs)
		{
			if (mob->IsDefeated() == false)
			{
				cleared = false;
			}
		}

		if (cleared)
		{
			bRoomCleared = true;
			OnRoomCleared();
		}
	}

}

void ARoomLevel::SpawnLootOfType(TSubclassOf<ALootObject> LootType)
{
	ALootObject* spawnedLootObject = GetWorld()->SpawnActor<ALootObject>(LootType.Get());
	if (spawnedLootObject)
	spawnedLootObject->SetActorTransform(RoomTransform);
}