// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlaythroughGameData_Base.h"
#include "PlaythroughGameSubsystem.generated.h"

class ULevelStreamingDynamic;
class ARoomLevel;
class AMob_Base;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlaythroughStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoomCleared);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossRoomLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeathCallback);

/**
 * Manages the state of the current playthrough : the level we're currently in, what bosses we've defeated, what level
 * to load next, what items & powerups we have...
 */
UCLASS()
class SWORDSBOTS_API UPlaythroughGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	UPlaythroughGameSubsystem();

	void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	void StartNewPlaythrough();
	UFUNCTION(BlueprintCallable)
	void LoadNextLevel();

	/// <summary>
	/// Generates a suitable boss arena for the current layer and teleports the player to it.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void GenerateAndTeleportToBossArena();

	UFUNCTION(BlueprintCallable)
	void StartBossFight(AMob_Base* BossMob);
	UFUNCTION(BlueprintCallable)
	AMob_Base* GetCurrentBossMob();
	UFUNCTION(BlueprintCallable)
	bool IsInBossFight();

	void SignalRoomCleared(ARoomLevel* ClearedRoom, bool bossRoom = false);
	void SignalPlayerDied();

	// Playthrough events
	UPROPERTY(BlueprintAssignable)
	FOnPlaythroughStarted PlaythroughStartedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnRoomCleared RoomClearedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnBossRoomLoaded BossRoomLoadedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeathCallback PlayerDiedDelegate;

private:

	UFUNCTION()
	void OnBossRoomLoaded();

	// Game data
	UPlaythroughGameData_Base* PlaythroughGameData;

	// "Special" levels
	TAssetPtr<UWorld> MainMenuLevel;

	// Layers
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Playthrough | Level Layers", meta = (AllowPrivateAccess = "true"))
	TArray<ULevelLayer*> PlaythroughLayers;

	// Current playthrough state data
	int CurrentPlaythroughLayerIndex;
	ULevelStreamingDynamic* LastSpawnedBossArena;
	AMob_Base* CurrentBossMob;
};
