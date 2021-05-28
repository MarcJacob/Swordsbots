// Fill out your copyright notice in the Description page of Project Settings.

#include "MobAerialState.h"
#include "../../Animation/MobAnimInstance.h"


ConcurrentSwordsbotControlProfileEnum UMobAerialState::GetInitialControlProfile()
{
	return DISALLOWS_ALL;
}

void UMobAerialState::OnBegin()
{
	GetControlledMob()->GetAnimInstance()->UpdateSpeedParameters(0.f, 0.f);
	GetControlledMob()->GetAnimInstance()->OnJumpStarted();
}

void UMobAerialState::OnUpdate(float DeltaTime)
{
	if (GetControlledMob()->GetMovementComponent()->IsFalling() == false)
	{
		OnControlledMobLanded(FHitResult());
	}
}

void UMobAerialState::OnControlledMobLanded(const FHitResult& LandingHit)
{
	GetControlledMob()->GetAnimInstance()->OnLanded();
	EndState();
}