// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "../Pawns/Interfaces/RoomMob.h"
#include "RoomLevel.generated.h"

class APawn;
class ALootObject;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoomEventDelegate);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SWORDSBOTS_API ARoomLevel : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:

	ARoomLevel();

	void SpawnLootOfType(TSubclassOf<ALootObject> LootType);

	UFUNCTION(BlueprintNativeEvent)
	void OnRoomSpawned();
	UFUNCTION(BlueprintNativeEvent)
	void OnRoomActivated();
	UFUNCTION(BlueprintNativeEvent)
	void OnRoomCleared();

	void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FRoomEventDelegate OnSpawnedDelegate;
	UPROPERTY(BlueprintAssignable)
	FRoomEventDelegate OnActivatedDelegate;
	UPROPERTY(BlueprintAssignable)
	FRoomEventDelegate OnClearedDelegate;

	FTransform RoomTransform;

protected:

	TArray<IRoomMob*> RoomMobs;
	bool bActivated;
	bool bRoomCleared;
	
};
