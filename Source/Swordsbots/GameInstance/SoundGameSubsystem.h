// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundGameData_Base.h"
#include "MusicPiecePlayState.h"
#include "SoundGameSubsystem.generated.h"

class AMob_Base;
class IBossMob;
class UAudioComponent;

// Defines a level of priority the current music is playing at. This prevents, for example, the miniboss music from overriding the boss music
// if ever two enemies of these types get aggroed at the same time.
enum class MusicLevel
{
	AMBIENT,
	MINIBOSS,
	BOSS
};



/**
 * Manages the playing of music over the course of gameplay.
 */
UCLASS()
class SWORDSBOTS_API USoundGameSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	
public:

	USoundGameSubsystem();

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { return GetStatID(); }

	void PlayFXSound(USoundBase* FXSound, FVector Location);

	UFUNCTION(BlueprintCallable)
	void RegisterMinibossMusicTriggeringMob(AMob_Base* Mob);

	UFUNCTION(BlueprintCallable)
	void PlayBossMusicForMob(const FMusicPiece& BossMusic, TScriptInterface<IBossMob> BossMob);

	UFUNCTION(BlueprintCallable)
	void PlayPlayerDeadMusic();

private:

	void PlayMinibossMusic();
	void PlayAmbientMusic();

	void CheckShouldMinibossMusicEnd();
	void CheckShouldBossMusicEnd();

	TArray<AMob_Base*> MinibossMusicTriggeringMobs;

	TScriptInterface<IBossMob> CurrentBossMusicMob;

	MusicLevel CurrentMusicLevel;
	const FMusicPiece* CurrentMusicPiece;
	const FMusicPiece* NextMusicPiece;

	UPROPERTY()
		UMusicPiecePlayState* CurrentMusicPiecePlayState;
	UPROPERTY()
		UMusicPiecePlayState* PreviousMusicPiecePlayState;

	UPROPERTY()
	UAudioComponent* CurrentMusicComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Music", meta = (AllowPrivateAccess = "true"))
	USoundGameData_Base* MusicGameData;
};
