// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Animation/MobAnimInstance.h"
#include "Components/MobMovementComponent.h"
#include "Components/MobCombatComponent.h"
#include "Interfaces/RoomMob.h"
#include "Mob_Base.generated.h"

class UMobState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwordsbotOnDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSwordsbotOnHurtDelegate, float, Damage);

/**
 * A Mob is any active element in the world that is able to execute basic actions that can influence the rest of the world.
 * It defines the most basic interface elements like the player character or enemies have to support.
 */
UCLASS()
class SWORDSBOTS_API AMob_Base : public APawn, public IRoomMob
{
	GENERATED_BODY()

public:
	AMob_Base(const FObjectInitializer& ObjectInitializer);
	void PostInitializeComponents() override;
protected:
	void BeginPlay() override;
	
public:
	void Tick(float DeltaTime) override;

	// State machine management

	UFUNCTION(BlueprintCallable)
	void SetNextState(UMobState* NextState);


	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* DamageInstigator, AActor* DamageCauser);
	virtual void Kill() { bIsDead = true; }

	// RoomMob implementation

	virtual bool IsDefeated() override;

	// Mob Actions

	/// <summary>
	/// Makes the Mob attack in front, towards wherever the Mob is currently looking.
	/// If a State is a returned it is assumed the attack was successfully started. Otherwise the
	/// attack is assumed to have failed.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	virtual UMobState* Attack();

	/// <summary>
	/// Makes the Mob move in the given Direction.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	virtual void MoveTowards(FVector Direction);

	/// <summary>
	/// Makes the Mob jump, suddenly gaining upward momentum and allowing it to travel accross holes and elevation.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	virtual void Jump();

	UFUNCTION(BlueprintCallable)
	void LockRotationTo(FRotator Rotation);

	UFUNCTION(BlueprintCallable)
	void UnlockRotation();

	FORCEINLINE bool IsEnemyOf(AMob_Base* Other) { return Other->FactionID == 0 || Other->FactionID != this->FactionID; }
	FORCEINLINE bool IsAlive() { return !bIsDead; }

	FORCEINLINE UMobAnimInstance* GetAnimInstance() const { return AnimationInstance; }
	FORCEINLINE UMobMovementComponent* GetMovementComponent() const { return MovementComponent; }
	FORCEINLINE class UCapsuleComponent* GetCollisionComponent() { return CollisionComponent; }
	FORCEINLINE class USkeletalMeshComponent* GetMesh() { return MeshComponent; }
	FORCEINLINE class UMobCombatComponent* GetCombatComponent() { return CombatComponent; }

	FORCEINLINE const UMobState* GetCurrentState() { return CurrentState; }
	FORCEINLINE bool HasState() { return CurrentState != nullptr; }

	// Delegates

	UPROPERTY(BlueprintAssignable)
	FSwordsbotOnDeathDelegate OnDeathDelegate;
	UPROPERTY(BlueprintAssignable)
	FSwordsbotOnHurtDelegate OnHurtDelegate;

protected:


	
	UFUNCTION()
	void OnStruckInMelee(UMobCombatComponent* Striker, float Damage, EHitResponse HitResponse);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Visuals", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Swordsbot", meta = (AllowPrivateAccess = "true"))
	UMobMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Swordsbot", meta = (AllowPrivateAccess = "true"))
	UMobAnimInstance* AnimationInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UMobCombatComponent* CombatComponent;

	// State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Swordsbot | State", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	// STATS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Stats", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	// ALLEGIANCE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Allegiance", meta = (AllowPrivateAccess = "true"))
	int FactionID;

	// MISC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot | Misc", meta = (AllowPrivateAccess = "true"))
	float DespawnTimeAfterDeath;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mob | Stats", meta = (AllowPrivateAccess = "true"))
	bool bRagdollOnDeath = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mob | Stats", meta = (AllowPrivateAccess = "true"))
	bool bStunOnHit = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Swordsbot | State", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mob", meta = (AllowPrivateAccess = "true"))
	FName MobName;

	bool bIsRotationLocked; // When true, the mob will always (unless forced not to) try to be looking in the direction of LockedRotation.
	FRotator LockedRotation;

	// State machine data

	UMobState* NextState; // Defines what state to switch to when this isn't equal to CurrentState

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State Machine", meta = (AllowPrivateAccess = "true"))
	UMobState* CurrentState; // When not NULL, defines the behavior of the Swordsbot, possibly overriding default behavior entirely.

};
