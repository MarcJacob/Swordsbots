// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCameraComponent.h"
#include "../Pawns/Swordsbots/Swordsbot.h"
#include <Components/PostProcessComponent.h>

UGameplayCameraComponent::UGameplayCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	HurtPostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("Hurt Post Process Component"));
	DeadPostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("Dead Post Process Component"));

	HurtPostProcessComponent->SetupAttachment(this);
	DeadPostProcessComponent->SetupAttachment(this);
}

void UGameplayCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	ASwordsbot* controlledSwordsbot = Cast<ASwordsbot>(GetOwner());

	if (controlledSwordsbot != nullptr)
	{
		// Link to Swordsbot delegates to play camera animations.
		controlledSwordsbot->OnHurtDelegate.AddDynamic(this, &UGameplayCameraComponent::OnPlayerSwordsbotHurt);
		controlledSwordsbot->OnDeathDelegate.AddDynamic(this, &UGameplayCameraComponent::OnPlayerSwordsbotDied);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WARNING : Gameplay Camera Component cannot properly be set up if the Owner actor isn't a Swordsbot."));
	}

	if (HurtPostProcessComponent != nullptr)
	HurtPostProcessComponent->BlendWeight = 0.f;
	if (DeadPostProcessComponent != nullptr)
	DeadPostProcessComponent->BlendWeight = 0.f;

	NormalizedDeadPostProcessingWeight = -1.f;
}

void UGameplayCameraComponent::OnPlayerSwordsbotHurt(float Damage)
{
	NormalizedHurtPostProcessingWeight = 1.f;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Player Swordsbot hurt")));

}

void UGameplayCameraComponent::OnPlayerSwordsbotDied()
{
	NormalizedDeadPostProcessingWeight = 0.f;
}

void UGameplayCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DeadPostProcessComponent != nullptr && NormalizedDeadPostProcessingWeight >= 0.f)
	{
		NormalizedDeadPostProcessingWeight += DeltaTime;
		DeadPostProcessComponent->BlendWeight = NormalizedDeadPostProcessingWeight;
	}
	if (HurtPostProcessComponent != nullptr && NormalizedHurtPostProcessingWeight > 0.f)
	{
		NormalizedHurtPostProcessingWeight -= DeltaTime / HurtPostProcessingAnimationTime;
		HurtPostProcessComponent->BlendWeight = NormalizedHurtPostProcessingWeight;
	}
}