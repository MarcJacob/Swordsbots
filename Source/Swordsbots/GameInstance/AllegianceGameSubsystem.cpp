// Fill out your copyright notice in the Description page of Project Settings.


#include "AllegianceGameSubsystem.h"

UAllegianceGameSubsystem::UAllegianceGameSubsystem()
{
	// Link corresponding Data object.
	static ConstructorHelpers::FObjectFinder<UClass> dataFinder(TEXT("Class'/Game/Blueprints/GameData/GameData_Allegiance.GameData_Allegiance_C'"));
	if (dataFinder.Succeeded())
	{
		AllegianceData = Cast<UAllegianceGameData_Base>(CreateDefaultSubobject(TEXT("DataObject_Allegiance"), dataFinder.Object, dataFinder.Object, true, false));
	}
}

void UAllegianceGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (AllegianceData != nullptr)
	{
		SET_WARN_COLOR(COLOR_GREEN);
		UE_LOG(LogTemp, Log, TEXT("Loading Allegiance Data from Data Object %s"), *AllegianceData->GetName());
		SET_WARN_COLOR(COLOR_YELLOW);
		LoadedFactions = TArray<Faction>(AllegianceData->Factions);
	}

}

Faction UAllegianceGameSubsystem::GetFactionData(int FactionID)
{
	if (FactionID < LoadedFactions.Num())
	{
		return LoadedFactions[FactionID];
	}
	else
	{
		if (LoadedFactions.Num() > 0)
		{
			return LoadedFactions[0];
		}
		else
		{
			return Faction();
		}
	}
}