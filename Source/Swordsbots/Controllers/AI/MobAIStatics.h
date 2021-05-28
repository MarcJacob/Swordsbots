// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AMob_Base;

/**
 * Static functions for use by AI code.
 */
class SWORDSBOTS_API MobAIStatics
{
public:

	static AMob_Base* FindClosestTargetFor(AMob_Base* Mob, float Range);
};
