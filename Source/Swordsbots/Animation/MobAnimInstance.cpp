// Fill out your copyright notice in the Description page of Project Settings.


#include "MobAnimInstance.h"

void UMobAnimInstance::UpdateSpeedParameters(float NormalizedFSpeed, float NormalizedSSpeed)
{
	this->NormalizedForwardSpeed = NormalizedFSpeed;
	this->NormalizedSideSpeed = NormalizedSSpeed;
}

void UMobAnimInstance::OnAttackWithSpecificAnimation(UAnimMontage* SpecificAnimation)
{
	Montage_Play(SpecificAnimation);
}

