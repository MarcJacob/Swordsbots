// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordsbotMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>

void USwordsbotMovementComponent::AirControlsStateTick(float DeltaTime)
{

}

void USwordsbotMovementComponent::GroundedStateTick(float DeltaTime)
{
	// Check if we are too far from the ground

	if (GetLastGroundCheck().DistanceToGround - GetFeetZOffsetFromPivot() > 15.f)
	{
		bIsFalling = true;
		return;
	}

	// Now that we're here we know we are actually grounded. Snap the character to actually being on the ground.
	UpdatedComponent->AddWorldOffset(FVector::DownVector * (GetLastGroundCheck().DistanceToGround - GetFeetZOffsetFromPivot())); // No need to sweep.
	Velocity.Z = 0.f;
	// Root motion : In grounded mode we override normal movement input with root motion if there's any.

	FRootMotionMovementParams rootMotion = GetRootMotionSource()->ConsumeRootMotion();
	FVector rootMotionVelocity = GetOwner()->GetActorRotation().Quaternion() * rootMotion.RootMotionTransform.GetLocation() * RootMotionSpeedMultiplier * GetRootMotionSource()->GetRelativeScale3D().Size();
	rootMotionVelocity /= DeltaTime;
	if (rootMotionVelocity.IsNearlyZero())
	{
		FVector movementInputVector = ConsumeInputVector();
		if (movementInputVector.IsNearlyZero() == false)
		{
			movementInputVector.Z = 0.f;
			if (movementInputVector.Size() > 1.f) movementInputVector = movementInputVector.GetUnsafeNormal();
			NormalizedGroundedMovement = (FVector2D)movementInputVector;
			if (!bSprintDesired)
			{
				NormalizedGroundedMovement *= 0.75f;
			}
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


	float desiredXVelocity = 0.f;
	float desiredYVelocity = 0.f;

	if (bSprintDesired)
	{
		desiredXVelocity = NormalizedGroundedMovement.X * SprintSpeed;
		desiredYVelocity = NormalizedGroundedMovement.Y * SprintSpeed;
	}
	else
	{
		desiredXVelocity = NormalizedGroundedMovement.X / 0.75f * RunSpeed;
		desiredYVelocity = NormalizedGroundedMovement.Y / 0.75f * RunSpeed;
	}

	float maxSpeed = RunSpeed;
	if (bSprintDesired) maxSpeed = SprintSpeed;

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