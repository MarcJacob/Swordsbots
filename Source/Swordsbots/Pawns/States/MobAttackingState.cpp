// Fill out your copyright notice in the Description page of Project Settings.


#include "MobAttackingState.h"
#include "../../Animation/SwordsbotAnimInstance.h"
#include "../Mob_Base.h"

ConcurrentSwordsbotControlProfileEnum UMobAttackingState::GetInitialControlProfile()
{
	return DISALLOWS_ALL;
}

void UMobAttackingState::OnBegin()
{
	if (SpecificAnimationMontage == nullptr)
	{
		GetControlledMob()->GetAnimInstance()->OnAttack(Direction);
	}
	else
	{
		GetControlledMob()->GetAnimInstance()->OnAttackWithSpecificAnimation(SpecificAnimationMontage);
	}
	GetControlledMob()->GetCombatComponent()->DisableBlocking(); // TODO : Could implement chambering by waiting for a certain time before disabling blocking.
	GetControlledMob()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UMobAttackingState::OnMobAnimationNotify);
}

void UMobAttackingState::OnUpdate(float DeltaTime)
{

}

void UMobAttackingState::OnEnd()
{
	Super::OnEnd();
	GetControlledMob()->GetCombatComponent()->EnableBlocking();
	GetControlledMob()->GetAnimInstance()->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UMobAttackingState::OnMobAnimationNotify);

}

void UMobAttackingState::OnMobAnimationNotify(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == "Hit")
	{
		OnHitNotify();
	}
	else if (NotifyName == "ActionEnd")
	{
		OnActionEndNotify();
	}
}

void UMobAttackingState::OnBlocked()
{
	GetControlledMob()->GetAnimInstance()->OnBlocked();
}

void UMobAttackingState::OnParried()
{
	GetControlledMob()->GetAnimInstance()->OnParried();
}

void UMobAttackingState::OnHitNotify()
{
	UMobCombatComponent* combatComponent = GetControlledMob()->GetCombatComponent();

	TArray<HittableTargetScanResult> hittableActors = TArray<HittableTargetScanResult>();
	combatComponent->DetectHittableTargets(hittableActors);

	if (hittableActors.Num() > 0)
	{
		EHitResponse hitResponse = combatComponent->ProcessStrike(hittableActors[0].TargetActor, Direction, bIsBlockable, bIsParriable, DamageAmount);

		switch (hitResponse)
		{
		case(EHitResponse::HIT_TAKEN):

			break;
			// TODO : Implement blocking & Parrying
		case(EHitResponse::HIT_BLOCKED):
			OnBlocked();
			break;
		case(EHitResponse::HIT_PARRIED):
			OnParried();
			break;
		}
	}
}

void UMobAttackingState::OnActionEndNotify()
{
	EndState();
}