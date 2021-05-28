// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGuardDirection : uint8
{
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
};
UENUM(BlueprintType)
enum class EHitResponse : uint8
{
	HIT_TAKEN, // Hit was taken, attacker should just finish their attack animation
	HIT_BLOCKED, // Hit was blocked, attacker should get visual feedback
	HIT_PARRIED // Hit was parried, attacker should get visual feedback and be stunned for a short time
};