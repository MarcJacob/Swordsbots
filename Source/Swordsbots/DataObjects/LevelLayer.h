// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LootObject.h"
#include "LevelLayer.generated.h"

/**
 * Level Layers are data objects that describe an entire "layer" in a playthrough : the levels that can be chosen
 * as this layer's played level, the loot table for Room rewards, and the boss levels that can be chosen.
 */
UCLASS(Blueprintable)
class SWORDSBOTS_API ULevelLayer : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TAssetPtr<UWorld>> PossibleLevels;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TAssetPtr<UWorld>> PossibleBossLevels;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<ALootObject>> PossibleRoomLootRewards;

};
