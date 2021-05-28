// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../DataObjects/LevelLayer.h"
#include "PlaythroughGameData_Base.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SWORDSBOTS_API UPlaythroughGameData_Base : public UObject
{
	GENERATED_BODY()
	
public:

	// "Special" scenes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetPtr<UWorld> MainMenuLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ULevelLayer>> PlaythroughLayers;
};
