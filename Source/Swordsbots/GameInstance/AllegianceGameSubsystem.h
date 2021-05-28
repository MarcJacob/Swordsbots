// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AllegianceGameData_Base.h"
#include "AllegianceGameSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SWORDSBOTS_API UAllegianceGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	UAllegianceGameSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	Faction GetFactionData(int FactionID);

private:

	UAllegianceGameData_Base* AllegianceData;

	TArray<Faction> LoadedFactions;
};
