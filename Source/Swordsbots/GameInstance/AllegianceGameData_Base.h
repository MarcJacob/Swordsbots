// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AllegianceGameData_Base.generated.h"

USTRUCT(BlueprintType)
struct Faction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FactionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* FactionColor;
};

/**
 * Data block object for the Allegiance Game Subsystem.
 * Contains Faction data (Name, material...)
 */
UCLASS(Abstract, Blueprintable)
class SWORDSBOTS_API UAllegianceGameData_Base : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data | Factions")
	TArray<Faction> Factions;
};
