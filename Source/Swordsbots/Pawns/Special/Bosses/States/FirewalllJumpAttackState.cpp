// Fill out your copyright notice in the Description page of Project Settings.


#include "FirewallJumpAttackState.h"
#include "../../../Mob_Base.h"
#include "../../../../Animation/MobAnimInstance.h"
#include <Kismet/KismetMathLibrary.h>


void UFirewallJumpAttackState::SetJumpCurveData(AActor* TargetActor, float Height, float TravelTime, UCurveFloat* HeightCurve)
{
	Target = TargetActor;
	JumpHeight = Height;
	CurveTravelTime = TravelTime;
	CurveTravelRatio = 0;
	HeightRatioCurve = HeightCurve;
}

void UFirewallJumpAttackState::SetJumpCurveDataWithPosition(FVector TargetPosition, float Height, float TravelTime, UCurveFloat* HeightCurve)
{
	Target = nullptr;
	FinalJumpTargetPosition = TargetPosition;
	JumpHeight = Height;
	CurveTravelTime = TravelTime;
	CurveTravelRatio = 0;
	HeightRatioCurve = HeightCurve;
}

void UFirewallJumpAttackState::SetLookAtTargetPosition(bool LookAtTarget)
{
	bLookAtTargetPosition = LookAtTarget;
}

void UFirewallJumpAttackState::SetAttackDamage(int Damage)
{
	AttackDamage = Damage;
}

void UFirewallJumpAttackState::OnAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == "Jump") {
		bTravellingOnCurve = true;
		StartHeight = GetControlledMob()->GetActorLocation().Z;
		
		if (Target != nullptr)
		{
			FVector toTarget = Target->GetActorLocation() - JumpStartPosition;
			toTarget.Z = 0.f;
			toTarget.Normalize();
			
			FinalJumpTargetPosition = Target->GetActorLocation() - toTarget * 250.f;
		}
		// If Target is null, then we consider the FinalJumpTargetPosition to already be calculated.

		if (bLookAtTargetPosition)
		{
			FVector toTargetPos = FinalJumpTargetPosition - JumpStartPosition;
			GetControlledMob()->SetActorRotation(toTargetPos.Rotation());
		}

	}
	else if (NotifyName == "EndAction")
	{
		EndState();
	}
	else if (NotifyName == "Hit")
	{
		TArray<HittableTargetScanResult> hittables = TArray<HittableTargetScanResult>();
		GetControlledMob()->GetCombatComponent()->DetectHittableTargets(hittables);

		for (HittableTargetScanResult scanResult : hittables)
		{
			GetControlledMob()->GetCombatComponent()->ProcessStrike(scanResult.TargetActor, EGuardDirection::BOTTOM_RIGHT, false, false, AttackDamage);
		}
	}
}

void UFirewallJumpAttackState::OnBegin()
{
	JumpStartPosition = GetControlledMob()->GetActorLocation();

	if (bLookAtTargetPosition && Target != nullptr)
	{
		FVector toTarget = Target->GetActorLocation() - JumpStartPosition;
		toTarget.Z = 0.f;
		toTarget.Normalize();
		GetControlledMob()->SetActorRotation(toTarget.Rotation());
	}

	GetControlledMob()->GetCombatComponent()->DisableBlocking();
	GetControlledMob()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UFirewallJumpAttackState::OnAnimNotify);
}

void UFirewallJumpAttackState::OnEnd()
{
	GetControlledMob()->GetCombatComponent()->EnableBlocking();
	GetControlledMob()->GetAnimInstance()->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UFirewallJumpAttackState::OnAnimNotify);
}

void UFirewallJumpAttackState::OnUpdate(float DeltaTime)
{
	if (bTravellingOnCurve && CurveTravelRatio < 1.f)
	{
		CurveTravelRatio += DeltaTime / CurveTravelTime;
		if (CurveTravelRatio > 1.f) CurveTravelRatio = 1.f;
		// lerp X/Y position by travel ratio and determine Z position using Highpoint and Startpoint.
		float heightRatio = HeightRatioCurve->GetFloatValue(CurveTravelRatio);

		float zPosition;
		if (CurveTravelRatio < 0.5f)
		{
			zPosition = UKismetMathLibrary::Lerp(StartHeight, StartHeight + JumpHeight, heightRatio);
		}
		else
		{
			zPosition = UKismetMathLibrary::Lerp(FinalJumpTargetPosition.Z, StartHeight + JumpHeight, heightRatio);
		}

		FVector finalPositionVector = UKismetMathLibrary::VLerp(JumpStartPosition, FinalJumpTargetPosition, CurveTravelRatio);
		finalPositionVector.Z = zPosition;

		GetControlledMob()->SetActorLocation(finalPositionVector);
	}
}