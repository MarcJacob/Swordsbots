// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Mob_Base.h"
#include "../../Interfaces/BossMob.h"
#include "Firewall_Mob_Base.generated.h"

class AFirewallBossController;
class UFirewallAnimInstance;
class UMobStunUntilNotifyState;
class UFirewallJumpAttackState;

/**
 * 
 */
UCLASS()
class SWORDSBOTS_API AFirewall_Mob_Base : public AMob_Base, public IBossMob
{
	GENERATED_BODY()
	
public:

	virtual bool IsDefeated() override;
	UFUNCTION()
	virtual void StartNextPhase() override;

	virtual void PossessedBy(AController* PossessedBy) override;

	// Mob Actions

	virtual UMobState* Attack() override;
	virtual void Kill() override; // Override to play a specific animation.

	// Firewall Specific Actions

	UFUNCTION(BlueprintCallable)
	UMobState* AttackLeft();
	UFUNCTION(BlueprintCallable)
	UMobState* AttackRight();
	UFUNCTION(BlueprintCallable)
	UFirewallJumpAttackState* JumpAttack(AActor* Target);
	UFUNCTION(BlueprintCallable)
	UFirewallJumpAttackState* BackflipTo(FVector TargetPosition);
	UFUNCTION(BlueprintCallable)
	void SwitchOnGuardMode(bool On); // Triggers the Firewall to visibly hold her shield up and block every attack on her front while not attacking.
	
	UMobStunUntilNotifyState* Kneel();
	void StandUp();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss", meta = (AllowPrivateAccess = "true"))
		int CurrentPhase = 0;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firewall", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* JumpHeightCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firewall", meta = (AllowPrivateAccess = "true"))
	int DamagePerAttack = 1;

	AFirewallBossController* BossController;
	UFirewallAnimInstance* BossAnimInstance;
};
