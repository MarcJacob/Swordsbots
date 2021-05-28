// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaythroughGameSubsystem.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Engine/LevelStreaming.h>
#include <Engine/LevelStreamingDynamic.h>
#include "GameFramework/PlayerStart.h"
#include "SoundGameSubsystem.h"
#include "../Map/RoomLevel.h"
#include "../Pawns/Mob_Base.h"

#define BOSS_ROOM_SPAWN_COORDS FVector(0, 0, -10000)


UPlaythroughGameSubsystem::UPlaythroughGameSubsystem()
{
	// Link corresponding Data object.
	static ConstructorHelpers::FObjectFinder<UClass> dataFinder(TEXT("Class'/Game/Blueprints/GameData/GameData_Playthrough.GameData_Playthrough_C'"));
	if (dataFinder.Succeeded())
	{
		PlaythroughGameData = Cast<UPlaythroughGameData_Base>(CreateDefaultSubobject(TEXT("DataObject_Playthrough"), dataFinder.Object, dataFinder.Object, true, false));
	}
}

void UPlaythroughGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (PlaythroughGameData != nullptr)
	{
		SET_WARN_COLOR(COLOR_GREEN);
		UE_LOG(LogTemp, Log, TEXT("Loading Playthrough Data from Data Object %s"), *PlaythroughGameData->GetName());
		SET_WARN_COLOR(COLOR_YELLOW);
		
		for (TSubclassOf<ULevelLayer> layerClass : PlaythroughGameData->PlaythroughLayers)
		{
			ULevelLayer* loadedLayer = NewObject<ULevelLayer>(this, layerClass.Get());
			PlaythroughLayers.Add(loadedLayer);
		}

		MainMenuLevel = PlaythroughGameData->MainMenuLevel;
	}
}

void UPlaythroughGameSubsystem::StartNewPlaythrough()
{
	CurrentPlaythroughLayerIndex = -1;
	LoadNextLevel();
	PlaythroughStartedDelegate.Broadcast();
}

void UPlaythroughGameSubsystem::LoadNextLevel()
{
	CurrentPlaythroughLayerIndex++;
	if (CurrentPlaythroughLayerIndex >= PlaythroughLayers.Num())
	{
		// Playthrough ended, load main menu.
		// TODO : Play some sort of end cutscene.
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MainMenuLevel);
	}
	else
	{
		int randomLevelID = UKismetMathLibrary::RandomIntegerInRange(0, PlaythroughLayers[CurrentPlaythroughLayerIndex]->PossibleLevels.Num() - 1);
		TAssetPtr<UWorld> ChosenLevelForLayer = PlaythroughLayers[CurrentPlaythroughLayerIndex]->PossibleLevels[randomLevelID];
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), ChosenLevelForLayer);
	}
}

void UPlaythroughGameSubsystem::SignalRoomCleared(ARoomLevel* ClearedRoom, bool bossRoom)
{
	int possibleLootObjectsCount = PlaythroughLayers[CurrentPlaythroughLayerIndex]->PossibleRoomLootRewards.Num();
	if (possibleLootObjectsCount > 0)
	{
		TSubclassOf<ALootObject> roomReward = PlaythroughLayers[CurrentPlaythroughLayerIndex]->PossibleRoomLootRewards[UKismetMathLibrary::RandomIntegerInRange(0, possibleLootObjectsCount - 1)];
		ClearedRoom->SpawnLootOfType(roomReward);
	}

	RoomClearedDelegate.Broadcast();
}

void UPlaythroughGameSubsystem::SignalPlayerDied()
{
	GetGameInstance()->GetSubsystem<USoundGameSubsystem>()->PlayPlayerDeadMusic();

	PlayerDiedDelegate.Broadcast();
}

void UPlaythroughGameSubsystem::GenerateAndTeleportToBossArena()
{
	ULevelLayer* currentLayer = PlaythroughLayers[CurrentPlaythroughLayerIndex];

	FString RoomPath = currentLayer->PossibleBossLevels[UKismetMathLibrary::RandomIntegerInRange(0, currentLayer->PossibleBossLevels.Num() - 1)].GetLongPackageName();

	bool loadSuccessful;
	// Spawn boss room at specific coordinates (see DEFINE above). This is where the center of the Boss Room will be located.
	LastSpawnedBossArena = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), RoomPath, BOSS_ROOM_SPAWN_COORDS, FQuat::Identity.Rotator(), loadSuccessful);
	LastSpawnedBossArena->bShouldBlockOnLoad = true;
	LastSpawnedBossArena->SetShouldBeLoaded(true);
	if (loadSuccessful)
	{
		LastSpawnedBossArena->OnLevelLoaded.AddDynamic(this, &UPlaythroughGameSubsystem::OnBossRoomLoaded);
	}
}

void UPlaythroughGameSubsystem::StartBossFight(AMob_Base* BossMob)
{
	CurrentBossMob = BossMob;
}

AMob_Base* UPlaythroughGameSubsystem::GetCurrentBossMob()
{
	return CurrentBossMob;
}

bool UPlaythroughGameSubsystem::IsInBossFight()
{
	return IsValid(CurrentBossMob) && CurrentBossMob->IsAlive();
}

void UPlaythroughGameSubsystem::OnBossRoomLoaded()
{
	// TODO : Find some way of marking where the player should arrive within the boss room and teleport them there instead.
	// For now the player gets placed at the center coordinate of the Boss Room.

	
	ALevelScriptActor* levelScriptActor = LastSpawnedBossArena->GetLevelScriptActor();
	ARoomLevel* roomLevel = Cast<ARoomLevel>(levelScriptActor);
	roomLevel->OnRoomActivated();

	FVector playerPos = BOSS_ROOM_SPAWN_COORDS;

	// Attempt to find a Player Start actor instead
	for (AActor* actor : LastSpawnedBossArena->GetLoadedLevel()->Actors)
	{
		if (IsValid(actor) && actor->GetClass() == APlayerStart::StaticClass())
		{
			playerPos = actor->GetRootComponent()->GetRelativeLocation() + LastSpawnedBossArena->LevelTransform.GetLocation();
			break;
		}
	}

	UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->SetActorLocation(playerPos);
}