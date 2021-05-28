// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Map/InteractableMapElement.h"
#include <Components/PrimitiveComponent.h>
#include <Components/SceneComponent.h>
#include "LootObject.generated.h"

UCLASS()
class SWORDSBOTS_API ALootObject : public AInteractableMapElement
{
	GENERATED_BODY()
	
public:	
	ALootObject();

protected:

	void BeginPlay();

	virtual void OnInteraction_Implementation(AController* UserController, AActor* User, const FHitResult& UserToInteractable) override;

private:

	UFUNCTION()
	void OnPickupVolumeOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Item | Properties", meta = (AllowPrivateAccess = "true"))
	bool bAutoPickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Item", meta = (AllowPrivateAccess = "true"))
	UPrimitiveComponent* PickupVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Item", meta = (AllowPrivateAccess = "true"))
	USceneComponent* LootObjectTransform;

};
