// Fill out your copyright notice in the Description page of Project Settings.


#include "MobDeadState.h"
#include "../Mob_Base.h"
#include "../../Animation/MobAnimInstance.h"
#include <Components/CapsuleComponent.h>


ConcurrentSwordsbotControlProfileEnum UMobDeadState::GetInitialControlProfile()
{
	return DISALLOWS_ALL;
}

void UMobDeadState::SetDespawnTime(float DespawnTime)
{
	TimeUntilDespawn = DespawnTime;
}

void UMobDeadState::SetTurnToRagdoll(bool TurnToRagdoll)
{
	bTurnToRagdoll = TurnToRagdoll;
}

void UMobDeadState::OnBegin()
{
	GetControlledMob()->GetAnimInstance()->OnDeath();
	GetControlledMob()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UMobDeadState::OnAnimNotify);

	GetControlledMob()->GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (bTurnToRagdoll)
	{
		GetControlledMob()->GetMesh()->SetCollisionProfileName("Ragdoll");
		GetControlledMob()->GetMesh()->SetSimulatePhysics(true);
	}
}

void UMobDeadState::OnUpdate(float DeltaTime)
{
	if (TimeUntilDespawn >= 0)
	{
		TimeUntilDespawn -= DeltaTime;
		if (TimeUntilDespawn < 0.f)
		{
			GetControlledMob()->Destroy();
			EndState();
		}
	}
}

void UMobDeadState::OnAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == "RagdollStart")
	{
		GetControlledMob()->GetMesh()->SetCollisionProfileName("Ragdoll");
		GetControlledMob()->GetMesh()->SetSimulatePhysics(true);
	}
}