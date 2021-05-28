// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSwordsbot.h"
#include <Kismet/KismetMathLibrary.h>
#include "../../Controllers/Player/SwordsbotPlayerController.h"

void APlayerSwordsbot::ChangeMaxHealth(int MaxHealthDelta)
{
	int oldMaxHealth = MaxHealth;
	MaxHealth = UKismetMathLibrary::Max(1, MaxHealth + MaxHealthDelta);
	MaxHealthDelta = MaxHealth - oldMaxHealth;

	if (MaxHealthDelta > 0)
	{
		CurrentHealth += MaxHealthDelta;
	}

	MaxHealthChangedDelegate.Broadcast(MaxHealthDelta);
}

ASwordsbotPlayerController* APlayerSwordsbot::GetController()
{
	return Cast<ASwordsbotPlayerController>(Super::GetController());
}

