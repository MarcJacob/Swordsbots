// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableMapElement.generated.h"



UCLASS()
class SWORDSBOTS_API AInteractableMapElement : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableMapElement();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnInteraction(AController* UserController, AActor* User, const FHitResult& UserToInteractable);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable", meta = (AllowPrivateAccess = "true"))
	bool bCanInteract;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable", meta = (AllowPrivateAccess = "true"))
	FName ActionDesc;
};
