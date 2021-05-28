// Fill out your copyright notice in the Description page of Project Settings.


#include "LootObject.h"
#include <Components/BoxComponent.h>
#include "../Pawns/Swordsbots/Swordsbot.h"
#include "../Controllers/Player/SwordsbotPlayerController.h"

// Sets default values
ALootObject::ALootObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LootObjectTransform = CreateDefaultSubobject<USceneComponent>(TEXT("Loot Object Transform"));
	SetRootComponent(LootObjectTransform);
	PickupVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Pickup Volume"));
	PickupVolume->SetupAttachment(GetRootComponent());
}

void ALootObject::BeginPlay()
{
	Super::BeginPlay();

	PickupVolume->OnComponentBeginOverlap.AddDynamic(this, &ALootObject::OnPickupVolumeOverlapped);
}

void ALootObject::OnPickupVolumeOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bAutoPickup && bCanInteract)
	{
		ASwordsbot* swordsbot = Cast<ASwordsbot>(OtherActor);

		if (swordsbot != nullptr)
		{
			APlayerController* playerController = Cast<APlayerController>(swordsbot->GetController());
			if (playerController != nullptr)
			{
				OnInteraction(playerController, swordsbot, FHitResult());
			}
		}
	}
}

void ALootObject::OnInteraction_Implementation(AController* UserController, AActor* User, const FHitResult& UserToInteractable)
{
	Super::OnInteraction_Implementation(UserController, User, UserToInteractable);

	bCanInteract = false;
	Destroy();
}

