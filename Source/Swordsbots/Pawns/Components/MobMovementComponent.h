// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MobMovementComponent.generated.h"

struct FMovementGroundCheckResult
{
	float DistanceToGround; // If no ground was found will be equal to max scan distance
	FHitResult SourceHitResult;
};


/**
 * 
 */
UCLASS()
class SWORDSBOTS_API UMobMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Jump();
	void AssignRootMotionSourceMesh(USkeletalMeshComponent* RootMotionSource);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector2D GetGroundedNormalizedMovement() { return NormalizedGroundedMovement; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CanJump() { return Velocity.Z == 0.f; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetFeetZOffsetFromPivot() { return FeetZOffsetFromPivot * UpdatedComponent->GetComponentScale().Z; }

	virtual bool IsFalling() { return bIsFalling; }

protected:

	virtual void GroundedStateTick(float DeltaTime);
	virtual void FreeFallingStateTick(float DeltaTime);

	FORCEINLINE FMovementGroundCheckResult GetLastGroundCheck() { return LatestGroundCheck; }
	FORCEINLINE USkeletalMeshComponent* GetRootMotionSource() { return RootMotionSourceMesh; }

	// TODO : Consider moving all this to private
	bool bIsFalling; 
	FVector2D NormalizedGroundedMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot Movement | Global", meta = (AllowPrivateAccess = "true"))
		float RootMotionSpeedMultiplier = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot Movement | Global", meta = (AllowPrivateAccess = "true"))
		float FeetZOffsetFromPivot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot Movement | Physics", meta = (AllowPrivateAccess = "true"))
		float GravityAccelerationPerSecond;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swordsbot Movement | Physics", meta = (AllowPrivateAccess = "true"))
		float TerminalVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swordsbot Movement | Normal Mode", meta = (AllowPrivateAccess = "true"))
		float RunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swordsbot Movement | Normal Mode", meta = (AllowPrivateAccess = "true"))
		float JumpForce;

private:

	void UpdateGroundCheck();

	// State

	
	FMovementGroundCheckResult LatestGroundCheck;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Swordsbot Movement", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* RootMotionSourceMesh;

};
