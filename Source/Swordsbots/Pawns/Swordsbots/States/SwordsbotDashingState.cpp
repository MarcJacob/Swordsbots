// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordsbotDashingState.h"
#include "../Swordsbot.h"
#include "../../../Animation/SwordsbotAnimInstance.h"
#include "../../Components/MobCombatComponent.h"

ConcurrentSwordsbotControlProfileEnum USwordsbotDashingState::GetInitialControlProfile()
{
	return DISALLOWS_ALL;
}

void USwordsbotDashingState::SetDashProperties(float DashSpeed, float DashTime, FVector DashDirection)
{
	SpeedPerSecond = DashSpeed;
	TotalTime = DashTime;
	Direction = DashDirection;
	ElapsedTime = 0.f;
}

void USwordsbotDashingState::OnBegin()
{
	GetTargetSwordsbot()->GetSwordsbotAnimation()->OnDash();
	GetTargetSwordsbot()->GetCombatComponent()->EnableBlocking();
	GetTargetSwordsbot()->GetCombatComponent()->ResetBlockingTime();
}

void USwordsbotDashingState::OnUpdate(float DeltaTime)
{

	GetTargetSwordsbot()->AddActorWorldOffset(Direction * SpeedPerSecond * DeltaTime, true);

	ElapsedTime += DeltaTime;
	if (ElapsedTime >= TotalTime)
	{
		EndState();
	}
}

void USwordsbotDashingState::OnEnd()
{
	Super::OnEnd();
	GetTargetSwordsbot()->GetCombatComponent()->DisableBlocking();
}
