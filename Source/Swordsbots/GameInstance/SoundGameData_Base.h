// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Components/AudioComponent.h"
#include "SoundGameData_Base.generated.h"

USTRUCT(BlueprintType)
struct FMusicPiece
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* Intro;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* Loop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* Outro;

	bool operator==(const FMusicPiece& Other)
	{
		return Intro == Other.Intro && Loop == Other.Loop && Other.Outro;
	}
};

/**
 * 
 */
UCLASS(Blueprintable)
class SWORDSBOTS_API USoundGameData_Base : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Game Data")
	USoundClass* FXSoundClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Game Data")
	USoundClass* MusicSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Game Data")
	USoundAttenuation* FXSoundAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Game Data")
	FMusicPiece MinibossMusicPiece;
};
