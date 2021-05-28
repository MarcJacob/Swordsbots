// Fill out your copyright notice in the Description page of Project Settings.


#include "BossRoomLevel.h"

void ABossRoomLevel::Tick(float DeltaTime)
{
	if (bActivated && !bRoomCleared)
	{
		if (BossMob->IsDefeated())
		{
			bRoomCleared = true;
			OnRoomCleared();
		}
	}
}