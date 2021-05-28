// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableMapElement.h"

// Sets default values
AInteractableMapElement::AInteractableMapElement()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanInteract = true;

}

// Called when the game starts or when spawned
void AInteractableMapElement::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableMapElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableMapElement::OnInteraction_Implementation(AController* UserController, AActor* User, const FHitResult& UserToInteractable)
{
	
}
