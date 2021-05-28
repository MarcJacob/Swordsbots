// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoomLevel.h"
#include "../Pawns/Interfaces/BossMob.h"
#include "BossRoomLevel.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SWORDSBOTS_API ABossRoomLevel : public ARoomLevel
{
	GENERATED_BODY()

public:

	void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Room", meta = (AllowPrivateAccess = "true"))
	TScriptInterface<IBossMob> BossMob;
};
