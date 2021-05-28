// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoundGameData_Base.h"
#include <Components/AudioComponent.h>
#include "MusicPiecePlayState.generated.h"


UCLASS()
class UMusicPiecePlayState : public UObject
{
	GENERATED_BODY()

		enum class MusicPiecePlayStateStage
	{
		AWAITING_DATA,
		PLAYING_INTRO,
		PLAYING_LOOP,
		PLAYING_OUTRO
	};

public:

	UMusicPiecePlayState();

	void Initialize(const FMusicPiece* MusicPiece, const UWorld* PlayingWorld);

	const FMusicPiece* PlayedMusicPiece;
	MusicPiecePlayStateStage CurrentStage;
	const UWorld* SoundPlayingWorld;

	UPROPERTY()
		UAudioComponent* IntroAudioComponent;
	UPROPERTY()
		UAudioComponent* LoopAudioComponent;
	UPROPERTY()
		UAudioComponent* OutroAudioComponent;

	void UpdatePlayState();
	void TransitionOut();
};

