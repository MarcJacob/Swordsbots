// Fill out your copyright notice in the Description page of Project Settings.


#include "MobStunUntilNotifyState.h"
#include "../../Animation/MobAnimInstance.h"
#include "../MobState.h"

ConcurrentSwordsbotControlProfileEnum UMobStunUntilNotifyState::GetInitialControlProfile()
{
	return DISALLOWS_ALL;
}

void UMobStunUntilNotifyState::SetNotifyName(FName Notify)
{
	NotifyName = Notify;
}

void UMobStunUntilNotifyState::SetStunnedVelocity(FVector StunnedVelocity)
{
	Velocity = StunnedVelocity;
}

void UMobStunUntilNotifyState::OnBegin()
{
	GetControlledMob()->GetCombatComponent()->DisableBlocking();
	GetControlledMob()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UMobStunUntilNotifyState::OnMobAnimationNotify);
}

void UMobStunUntilNotifyState::OnUpdate(float DeltaTime)
{
	GetControlledMob()->AddActorWorldOffset(Velocity * DeltaTime, true);
}

void UMobStunUntilNotifyState::OnEnd()
{
	Super::OnEnd();
	GetControlledMob()->GetCombatComponent()->EnableBlocking();
	GetControlledMob()->GetAnimInstance()->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UMobStunUntilNotifyState::OnMobAnimationNotify);
}

void UMobStunUntilNotifyState::OnMobAnimationNotify(FName Notify, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (this->NotifyName == NotifyName)
	{
		EndState();
	}
}