// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicPiecePlayState.h"
#include <Kismet/GameplayStatics.h>


UMusicPiecePlayState::UMusicPiecePlayState() : PlayedMusicPiece(nullptr), CurrentStage(MusicPiecePlayStateStage::AWAITING_DATA)
{

}

void UMusicPiecePlayState::Initialize(const FMusicPiece* MusicPiece, const UWorld* PlayingWorld)
{
	PlayedMusicPiece = MusicPiece;
	SoundPlayingWorld = PlayingWorld;

	// Start intro sound
	CurrentStage = MusicPiecePlayStateStage::PLAYING_INTRO;

	if (MusicPiece->Intro != nullptr)
	{
		IntroAudioComponent = UGameplayStatics::CreateSound2D(SoundPlayingWorld, MusicPiece->Intro, 1.f, 1.f, 0.f, nullptr, false, false);
		IntroAudioComponent->FadeIn(0.5f);
	}
}

void UMusicPiecePlayState::UpdatePlayState()
{
	switch (CurrentStage)
	{
	case(MusicPiecePlayStateStage::PLAYING_INTRO):
		// Simply wait for the intro to end, then proceed to play loop.
		if (!IsValid(IntroAudioComponent) || !IntroAudioComponent->IsPlaying())
		{
			// Intro audio is done or wasn't started in the first place - start loop.
			LoopAudioComponent = UGameplayStatics::CreateSound2D(SoundPlayingWorld, PlayedMusicPiece->Loop, 1.f, 1.f, 0.f, nullptr, false, false);
			LoopAudioComponent->Play();
			CurrentStage = MusicPiecePlayStateStage::PLAYING_LOOP;
		}

		break;
	case(MusicPiecePlayStateStage::PLAYING_LOOP):

		if (!LoopAudioComponent->IsPlaying() == false)
		{
			//UE_LOG(LogTemp, Error, TEXT("ERROR : Looped music stopped playing without the play state going to Outro : You probably forgot to set the music asset to loop !"));
			//TransitionOut();
		}

		break;
	case(MusicPiecePlayStateStage::PLAYING_OUTRO):

		break;
	}
}

void UMusicPiecePlayState::TransitionOut()
{
	if (CurrentStage > MusicPiecePlayStateStage::AWAITING_DATA && CurrentStage < MusicPiecePlayStateStage::PLAYING_OUTRO)
	{
		if (CurrentStage == MusicPiecePlayStateStage::PLAYING_INTRO)
		{
			IntroAudioComponent->FadeOut(0.25f, 0.f);
		}
		else if (CurrentStage == MusicPiecePlayStateStage::PLAYING_LOOP)
		{
			LoopAudioComponent->FadeOut(0.25f, 0.f);
		}

		CurrentStage = MusicPiecePlayStateStage::PLAYING_OUTRO;

		if (PlayedMusicPiece->Outro != nullptr)
		{
			OutroAudioComponent = UGameplayStatics::CreateSound2D(SoundPlayingWorld, PlayedMusicPiece->Outro, 1.f, 1.f, 0.f, nullptr, false, false);
			OutroAudioComponent->FadeIn(0.15f);
		}
	}
}