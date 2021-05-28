// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/ChildActorComponent.h>
#include "MapElementMover.generated.h"

UCLASS()
class SWORDSBOTS_API AMapElementMover : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapElementMover();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moved Element", meta = (AllowPrivateAccess = "true"))
	AActor* MovedActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moved Element", meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* OtherPositionLocator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moved Element", meta = (AllowPrivateAccess = "true"))
	float TravelTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moved Element", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* MovementCurve;

	float TravelStateNormalized;
	bool bIsReturning;
};
