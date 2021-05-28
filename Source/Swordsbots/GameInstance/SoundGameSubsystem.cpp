// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundGameSubsystem.h"
#include <Kismet/GameplayStatics.h>
#include <Components/AudioComponent.h>
#include "../Pawns/Mob_Base.h"
#include "../Pawns/Interfaces/BossMob.h"
#include "Components/AudioComponent.h"

USoundGameSubsystem::USoundGameSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UClass> dataFinder(TEXT("Class'/Game/Blueprints/GameData/GameData_Sound.GameData_Sound_C'"));

	if (dataFinder.Succeeded())
	{
		MusicGameData = Cast<USoundGameData_Base>(CreateDefaultSubobject(TEXT("DataObject_Sound"), dataFinder.Object, dataFinder.Object, true, false));
	}
}

void USoundGameSubsystem::PlayAmbientMusic()
{
	if (CurrentMusicLevel > MusicLevel::AMBIENT) return;

	NextMusicPiece = nullptr; // TODO : Add ambient music.
}

void USoundGameSubsystem::PlayMinibossMusic()
{
	if (CurrentMusicLevel >= MusicLevel::MINIBOSS) return;

	CurrentMusicLevel = MusicLevel::MINIBOSS;

	NextMusicPiece = &MusicGameData->MinibossMusicPiece;
}

void USoundGameSubsystem::PlayPlayerDeadMusic()
{
	CurrentBossMusicMob = nullptr;
	MinibossMusicTriggeringMobs.Empty();
	CurrentMusicLevel = MusicLevel::AMBIENT;
	PlayAmbientMusic();
}

void USoundGameSubsystem::PlayBossMusicForMob(const FMusicPiece& BossMusic, TScriptInterface<IBossMob> BossMob)
{
	if (CurrentMusicLevel < MusicLevel::BOSS || CurrentBossMusicMob == BossMob)
	{
		NextMusicPiece = &BossMusic;
		CurrentMusicLevel = MusicLevel::BOSS;
		CurrentBossMusicMob = BossMob;
	}
}

void USoundGameSubsystem::PlayFXSound(USoundBase* FXSound, FVector Location)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FXSound, Location, 1.f, 1.f, 0.1f, MusicGameData->FXSoundAttenuation);
}

void USoundGameSubsystem::RegisterMinibossMusicTriggeringMob(AMob_Base* Mob)
{
	if (IsValid(Mob) == false || Mob->IsAlive() == false) {

		UE_LOG(LogTemp, Error, TEXT("ERROR : Attempted to register invalid or dead mob as Miniboss Music Triggering."));
		return;
	}

	MinibossMusicTriggeringMobs.AddUnique(Mob);
	PlayMinibossMusic();
}

void USoundGameSubsystem::CheckShouldMinibossMusicEnd()
{
	bool shouldEnd = true;
	for (int i = 0; i < MinibossMusicTriggeringMobs.Num() && shouldEnd; i++)
	{
		if (IsValid(MinibossMusicTriggeringMobs[i]) && MinibossMusicTriggeringMobs[i]->IsAlive()) shouldEnd = false;
	}

	if (shouldEnd)
	{
		MinibossMusicTriggeringMobs.Empty();
		CurrentMusicLevel = MusicLevel::AMBIENT;
		PlayAmbientMusic();
	}
}

void USoundGameSubsystem::CheckShouldBossMusicEnd()
{
	if (!CurrentBossMusicMob || CurrentBossMusicMob->IsDefeated())
	{
		CurrentBossMusicMob = nullptr;
		CurrentMusicLevel = MusicLevel::AMBIENT;
		PlayAmbientMusic();
	}
}

void USoundGameSubsystem::Tick(float DeltaTime)
{
	switch (CurrentMusicLevel)
	{
	case(MusicLevel::MINIBOSS):
		CheckShouldMinibossMusicEnd();
		break;
	case(MusicLevel::AMBIENT):
		if (MinibossMusicTriggeringMobs.Num() > 0) // In the case where we've started and ended boss music while miniboss mobs are still alive.
		{
			PlayMinibossMusic();
		}
		break;
	case(MusicLevel::BOSS):
		CheckShouldBossMusicEnd();
		break;
	}

	if (PreviousMusicPiecePlayState != nullptr) PreviousMusicPiecePlayState->UpdatePlayState();
	if (CurrentMusicPiecePlayState != nullptr) CurrentMusicPiecePlayState->UpdatePlayState();


	// Check for necessity to start a new MusicPiecePlayState
	if (CurrentMusicPiece != NextMusicPiece)
	{
		// If previous was not null this is where it should finally be freed from memory.
		PreviousMusicPiecePlayState = CurrentMusicPiecePlayState;
		if (PreviousMusicPiecePlayState != nullptr)
		{
			PreviousMusicPiecePlayState->TransitionOut();
		}
		
		
		if (NextMusicPiece != nullptr)
		{
			CurrentMusicPiecePlayState = NewObject<UMusicPiecePlayState>();
			CurrentMusicPiecePlayState->Initialize(NextMusicPiece, GetWorld());
		}
		else
		{
			CurrentMusicPiecePlayState = nullptr;
		}
		CurrentMusicPiece = NextMusicPiece;
	}
}
