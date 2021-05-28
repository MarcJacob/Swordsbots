// Fill out your copyright notice in the Description page of Project Settings.


#include "Firewall_Mob_Base.h"
#include "../../MobState.h"
#include "../../States/MobStunUntilNotifyState.h"
#include "../../States/MobAttackingState.h"
#include "../../../Controllers/AI/Mobs/Bosses/FirewallBossController.h"
#include "../../../Animation/Bosses/FirewallAnimInstance.h"
#include "States/FirewallJumpAttackState.h"

bool AFirewall_Mob_Base::IsDefeated()
{
	return !IsAlive();
}

void AFirewall_Mob_Base::StartNextPhase()
{
	CurrentPhase++;
	BossController->OnControlledBossPhaseChange(CurrentPhase);
}

void AFirewall_Mob_Base::PossessedBy(AController* PossessedBy)
{
	BossAnimInstance = Cast<UFirewallAnimInstance>(GetAnimInstance());
	BossController = Cast<AFirewallBossController>(PossessedBy);
	Super::PossessedBy(PossessedBy);
	
	CurrentPhase = -1;
	StartNextPhase();
}

UMobState* AFirewall_Mob_Base::Attack()
{
	return AttackRight();
}

void AFirewall_Mob_Base::Kill()
{
	AMob_Base::Kill();
}

UMobState* AFirewall_Mob_Base::AttackLeft()
{
	if (GetCurrentState() != nullptr && GetCurrentState()->DisallowsCombatActions()) return nullptr;

	UMobAttackingState* attackingState = NewObject<UMobAttackingState>();
	attackingState->SetAttackDirection(EGuardDirection::TOP_LEFT);
	attackingState->SetDamage(DamagePerAttack);
	SetNextState(attackingState);

	return attackingState;
}

UMobState* AFirewall_Mob_Base::AttackRight()
{
	if (GetCurrentState() != nullptr && GetCurrentState()->DisallowsCombatActions()) return nullptr;

	UMobAttackingState* attackingState = NewObject<UMobAttackingState>();
	attackingState->SetAttackDirection(EGuardDirection::TOP_RIGHT);
	attackingState->SetDamage(DamagePerAttack);
	SetNextState(attackingState);

	return attackingState;
}

UFirewallJumpAttackState* AFirewall_Mob_Base::JumpAttack(AActor* Target)
{
	if (GetCurrentState() != nullptr && GetCurrentState()->DisallowsCombatActions()) return nullptr;

	BossAnimInstance->OnJumpAttack();
	UFirewallJumpAttackState* jumpAttackState = NewObject<UFirewallJumpAttackState>();
	jumpAttackState->SetAttackDamage(5.f);
	jumpAttackState->SetJumpCurveData(Target, 300.f, 1.f, JumpHeightCurve);

	SetNextState(jumpAttackState);

	return jumpAttackState;
}

UFirewallJumpAttackState* AFirewall_Mob_Base::BackflipTo(FVector TargetPosition)
{
	if (GetCurrentState() != nullptr && GetCurrentState()->DisallowsCombatActions()) return nullptr;

	BossAnimInstance->OnBackflip();
	UFirewallJumpAttackState* jumpAttackState = NewObject<UFirewallJumpAttackState>();
	jumpAttackState->SetAttackDamage(0);
	jumpAttackState->SetJumpCurveDataWithPosition(TargetPosition + GetMovementComponent()->GetFeetZOffsetFromPivot() * FVector::UpVector, 300.f, 1.2f, JumpHeightCurve);
	jumpAttackState->SetLookAtTargetPosition(false);

	FVector toTarget = TargetPosition - GetActorLocation();
	toTarget.Z = 0;
	SetActorRotation((-toTarget).Rotation());

	SetNextState(jumpAttackState);

	return jumpAttackState;
}

void AFirewall_Mob_Base::SwitchOnGuardMode(bool On)
{
	BossAnimInstance->SwitchGuardMode(On);
	if (On)
	{
		CombatComponent->EnableBlocking();
	}
	else
	{
		CombatComponent->DisableBlocking();
	}
}

UMobStunUntilNotifyState* AFirewall_Mob_Base::Kneel()
{
	if (GetCurrentState() != nullptr && GetCurrentState()->DisallowsStanceSwitch()) return nullptr;

	UMobStunUntilNotifyState* stunState = NewObject<UMobStunUntilNotifyState>();
	stunState->SetNotifyName("StandingUp");
	SetNextState(stunState);

	BossAnimInstance->OnKneel();

	return stunState;
}

void AFirewall_Mob_Base::StandUp()
{
	BossAnimInstance->OnStandup();
}

