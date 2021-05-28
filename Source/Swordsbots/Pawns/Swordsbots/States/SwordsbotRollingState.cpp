// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordsbotRollingState.h"
#include "../Swordsbot.h"
#include "../../../Animation/SwordsbotAnimInstance.h"

ConcurrentSwordsbotControlProfileEnum USwordsbotRollingState::GetInitialControlProfile()
{
	return DISALLOWS_ALL;
}

void USwordsbotRollingState::SetRollProperties(float RollSpeed, float RollTime, FVector RollDirection, float Invulnerability)
{
	SpeedPerSecond = RollSpeed;
	TotalTime = RollTime;
	ElapsedTime = 0.f;
	Direction = RollDirection;
	InvulnerableTime = Invulnerability;
}

void USwordsbotRollingState::OnBegin()
{
	GetTargetSwordsbot()->SetActorRotation(Direction.Rotation());

	GetTargetSwordsbot()->GetSwordsbotAnimation()->OnRoll();
}

void USwordsbotRollingState::OnUpdate(float DeltaTime)
{
	// TODO : Use movement component instead of directly adding world offset.
	GetTargetSwordsbot()->AddActorWorldOffset(Direction * SpeedPerSecond * DeltaTime, true);

	ElapsedTime += DeltaTime;
	if (ElapsedTime >= TotalTime)
	{
		EndState();
	}
}