// Fill out your copyright notice in the Description page of Project Settings.


#include "MobMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <Engine/CollisionProfile.h>

#define GROUND_CHECK_MAX_DISTANCE 100.f

void UMobMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateGroundCheck();

	if (bIsFalling)
	{
		FreeFallingStateTick(DeltaTime);
	}
	else
	{
		GroundedStateTick(DeltaTime);
	}

	FHitResult mainMovementHitResult = FHitResult();
	GetOwner()->AddActorWorldOffset(Velocity * DeltaTime, true, &mainMovementHitResult);
	if (mainMovementHitResult.IsValidBlockingHit())
	{
		FVector impactNormal = mainMovementHitResult.ImpactNormal;
		SlideAlongSurface(Velocity * DeltaTime, 1.f - mainMovementHitResult.Time, mainMovementHitResult.ImpactNormal, mainMovementHitResult);
		Velocity -= Velocity * impactNormal.GetAbs();
	}
}

void UMobMovementComponent::AssignRootMotionSourceMesh(USkeletalMeshComponent* RootMotionSource)
{
	RootMotionSourceMesh = RootMotionSource;
}

void UMobMovementComponent::Jump()
{
	Velocity.Z = UKismetMathLibrary::Max(Velocity.Z, JumpForce);
	bIsFalling = true;
}

void UMobMovementComponent::GroundedStateTick(float DeltaTime)
{
	// Check if we are too far from the ground

	if (LatestGroundCheck.DistanceToGround - GetFeetZOffsetFromPivot() > 15.f)
	{
		bIsFalling = true;
		return;
	}

	// Now that we're here we know we are actually grounded. Snap the character to actually being on the ground.
	UpdatedComponent->AddWorldOffset(FVector::DownVector * (LatestGroundCheck.DistanceToGround - GetFeetZOffsetFromPivot())); // No need to sweep.
	Velocity.Z = 0.f;
	// Root motion : In grounded mode we override normal movement input with root motion if there's any.

	FRootMotionMovementParams rootMotion = RootMotionSourceMesh->ConsumeRootMotion();
	FVector rootMotionVelocity = GetOwner()->GetActorRotation().Quaternion() * rootMotion.RootMotionTransform.GetLocation() * RootMotionSpeedMultiplier * RootMotionSourceMesh->GetRelativeScale3D().Size();
	rootMotionVelocity /= DeltaTime;
	if (rootMotionVelocity.IsNearlyZero())
	{
		FVector movementInputVector = ConsumeInputVector();
		if (movementInputVector.IsNearlyZero() == false)
		{
			movementInputVector.Z = 0.f;
			if (movementInputVector.Size() > 1.f) movementInputVector = movementInputVector.GetUnsafeNormal();
			NormalizedGroundedMovement = (FVector2D)movementInputVector;
		}
		else
		{
			NormalizedGroundedMovement = FVector2D::ZeroVector;
		}
	}
	else
	{
		NormalizedGroundedMovement.X = rootMotionVelocity.X / RunSpeed;
		NormalizedGroundedMovement.Y = rootMotionVelocity.Y / RunSpeed;
	}

	float desiredXVelocity = NormalizedGroundedMovement.X * RunSpeed;
	float desiredYVelocity = NormalizedGroundedMovement.Y * RunSpeed;

	float maxSpeed = RunSpeed;

	// Apply desired velocity
	{
		if (!UKismetMathLibrary::NearlyEqual_FloatFloat(desiredXVelocity, Velocity.X))
			if (UKismetMathLibrary::Abs(desiredXVelocity - Velocity.X) < maxSpeed)
			{
				Velocity.X = desiredXVelocity;
			}
			else
			{
				if (Velocity.X < desiredXVelocity)
				{
					Velocity.X += UKismetMathLibrary::Min(maxSpeed, desiredXVelocity - Velocity.X);
				}
				else if (Velocity.X > desiredXVelocity)
				{
					Velocity.X += UKismetMathLibrary::Max(-maxSpeed, desiredXVelocity - Velocity.X);
				}
			}
		Velocity.X = UKismetMathLibrary::FClamp(Velocity.X, -maxSpeed, maxSpeed);

		if (!UKismetMathLibrary::NearlyEqual_FloatFloat(desiredYVelocity, Velocity.Y))
			if (UKismetMathLibrary::Abs(desiredYVelocity - Velocity.Y) < maxSpeed)
			{
				Velocity.Y = desiredYVelocity;
			}
			else
			{
				if (Velocity.Y < desiredYVelocity)
				{
					Velocity.Y += UKismetMathLibrary::Min(maxSpeed, desiredYVelocity - Velocity.Y);
				}
				else if (Velocity.Y > desiredYVelocity)
				{
					Velocity.Y += UKismetMathLibrary::Max(-maxSpeed, desiredYVelocity - Velocity.Y);
				}
			}
		Velocity.Y = UKismetMathLibrary::FClamp(Velocity.Y, -maxSpeed, maxSpeed);
	}
}

void UMobMovementComponent::FreeFallingStateTick(float DeltaTime)
{
	// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHH !!!

	// Check if we're close enough to the ground to revert to ground control.
	if (Velocity.Z <= 0 && LatestGroundCheck.DistanceToGround - GetFeetZOffsetFromPivot() < 5.f)
	{
		bIsFalling = false;
		return;
	}

	// Apply gravity
	if (Velocity.Z > -TerminalVelocity)
	{
		Velocity.Z -= GravityAccelerationPerSecond * DeltaTime;
	}

}



void UMobMovementComponent::UpdateGroundCheck()
{
	LatestGroundCheck = FMovementGroundCheckResult();

	FVector groundCheckStartPoint = UpdatedComponent->GetComponentLocation();
	FCollisionShape shape = FCollisionShape();
	shape.ShapeType = ECollisionShape::Capsule;
	shape.Capsule.Radius = 34.f;
	shape.Capsule.HalfHeight = 90.f;

	TArray<FHitResult> hitResults = TArray<FHitResult>();

	FCollisionQueryParams params = FCollisionQueryParams();
	params.bFindInitialOverlaps = true;
	params.AddIgnoredActor(UpdatedComponent->GetOwner());
	if (GetWorld()->SweepMultiByChannel(hitResults, groundCheckStartPoint, groundCheckStartPoint + FVector::DownVector * GROUND_CHECK_MAX_DISTANCE, FQuat::Identity, ECollisionChannel::ECC_Pawn, shape, params))
	{
		for (int i = 0; i < hitResults.Num(); i++)
		{
			if (hitResults[i].Component.Get()->GetCollisionProfileName() == UCollisionProfile::BlockAll_ProfileName)
			{
				LatestGroundCheck.DistanceToGround = FVector::Dist(groundCheckStartPoint, hitResults[i].ImpactPoint);
				LatestGroundCheck.SourceHitResult = hitResults[i];
				return;
			}
		}
	}

	// If we reached here, then no suitable ground object was found.
	LatestGroundCheck.DistanceToGround = GROUND_CHECK_MAX_DISTANCE + shape.Capsule.HalfHeight;

}