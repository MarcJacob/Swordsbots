// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MobCombatComponentEnums.h"
#include "MobCombatComponent.generated.h"

// Sub-structures and class pre-declaration

struct HittableTargetScanResult
{
	AActor* TargetActor;
	float TargetDistance;

	HittableTargetScanResult(AActor* target, float distance)
	{
		TargetActor = target;
		TargetDistance = distance;
	}

	bool operator<(const HittableTargetScanResult& b) const
	{
		return TargetDistance < b.TargetDistance;
	}
};

class USoundCue;
class UParticleSystem;
class UMobCombatComponent;
class AMob_Base;

// Delegates for use by owning actor and possibly other game systems.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FStruckInMeleeDelegate, UMobCombatComponent*, Striker, float, DamageAmount, EHitResponse, HitResponse); // TODO : Replace the float by a proper FDamageEvent subtype.

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
/// <summary>
/// Allows a pawn to fight in melee combat - detecting potential targets, striking them & blocking / parrying hits.
/// Communicates back with its owner through events (observer pattern) and calling Actor functions like TakeDamage().
/// </summary>
class SWORDSBOTS_API UMobCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMobCombatComponent();
protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void DetectHittableTargets(TArray<HittableTargetScanResult>& HittableTargets);

	EHitResponse ProcessStrike(AActor* Target, EGuardDirection HitDirection, bool IsBlockable, bool IsParriable, int DamageAmount);
	EHitResponse ReceiveStrike(UMobCombatComponent* Attacker, EGuardDirection AttackDirection, bool IsBlockable, bool IsParriable, int DamageAmount);

	FORCEINLINE void EnableBlocking() { CanBlockStack--; }
	FORCEINLINE void DisableBlocking() { CanBlockStack++; if (!CanBlock()) BlockingTime = 0.f; }
	FORCEINLINE bool CanBlock() { return bCanEverBlock && CanBlockStack < 0; }
	FORCEINLINE void ResetBlockingTime() { BlockingTime = 0.f; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGuardDirection CurrentGuardDirection;

	// Delegates
	UPROPERTY(BlueprintAssignable)
	FStruckInMeleeDelegate OnStruckInMelee;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float HitRange; // TODO : Move to weapon property
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float HitAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ParryTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bCanEverBlock = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bFriendlyFireEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bOmniDirectionalBlocking = false;

	// FX Data >>>>>> TODO : Move to Actor class, or to a specific FX component ?

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | FX | Sounds", meta = (AllowPrivateAccess = "true"))
	TArray<USoundCue*> OnParriedSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | FX | Sounds", meta = (AllowPrivateAccess = "true"))
	TArray<USoundCue*> OnBlockedSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | FX | Sounds", meta = (AllowPrivateAccess = "true"))
	TArray<USoundCue*> OnStruckSounds;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | FX | Particle Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* OnParriedParticleSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | FX | Particle Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* OnBlockedParticleSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | FX | Particle Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* OnStruckParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | FX", meta = (AllowPrivateAccess = "true"))
	FVector CombatFXOffset;

	// <<<<<< FX Data

	AMob_Base* OwnerMob;

	// STATE
	int CanBlockStack; // Whether the linked pawn is currently able to block with its current guard. Blocks will only be checked
	// if the stack is below 0.
	float BlockingTime; // How long since we last switched from being unable to block to being able to. Determines whether
	// the hit response will be a PARRY or a BLOCK.
	bool bCanBeTargeted; // Whether this combat component is eligible to being targeted for a strike.
};
