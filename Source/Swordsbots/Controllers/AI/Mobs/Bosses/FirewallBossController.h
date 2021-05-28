// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MobAIController_Base.h"
#include "../../../../Pawns/Special/Bosses/Firewall_Mob_Base.h"
#include "FirewallBossController.generated.h"

class ASwordsbot;
class AFirewall_Mob_Base;

/**
 * 
 */
UCLASS()
class SWORDSBOTS_API AFirewallBossController : public AMobAIController_Base
{
	GENERATED_BODY()
	
public:

	void OnControlledBossPhaseChange(int newPhase);

	UFUNCTION(BlueprintCallable)
	void AssignTarget(ASwordsbot* TargetSwordsbot) { Target = TargetSwordsbot; }

	UFUNCTION(BlueprintCallable)
	void AssignPossibleJumpPositions(TArray<UPrimitiveComponent*> PossibleJumpZones);

	UFUNCTION(BlueprintCallable)
	FVector GetSuitableJumpPosition(FVector FromPosition);

	void Tick(float DeltaTime) override;
	void OnPossess(APawn* Possessed) override;

private:

	void ManageDistanceToTarget(float DeltaTime);
	void ManageCombatPatterns(float DeltaTime);
	void ChooseNextCombatPattern();

	UFUNCTION()
	void OnTargetDeath();


	AFirewall_Mob_Base* ControlledFirewall;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Firewall", meta =(AllowPrivateAccess = "true"))
	ASwordsbot* Target;

	TArray<UPrimitiveComponent*> PossibleJumpZones;
	
	int CurrentPhase = 0;
	float TimeUntilNextCombatPattern;

	bool UsingLongRangePattern;
	int CurrentCombatPatternIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firewall", meta = (AllowPrivateAccess = "true"))
		TArray<FMobAIPattern> MeleeRangeCombatPatterns;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firewall", meta = (AllowPrivateAccess = "true"))
		TArray<FMobAIPattern> LongRangeCombatPatterns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firewall", meta = (AllowPrivateAccess = "true"))
	float LongRangeAttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firewall", meta = (AllowPrivateAccess = "true"))
	float MeleeRangeAttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firewall", meta = (AllowPrivateAccess = "true"))
	float AverageTimeBetweenCombatPatterns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firewall", meta = (AllowPrivateAccess = "true"))
	float TimeBetweenCombatPatternsRandomVariance;
};
