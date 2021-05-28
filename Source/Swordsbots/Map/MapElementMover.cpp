// Fill out your copyright notice in the Description page of Project Settings.


#include "MapElementMover.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
AMapElementMover::AMapElementMover()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);

	OtherPositionLocator = CreateDefaultSubobject<UChildActorComponent>(TEXT("Other Position Locator"));
	OtherPositionLocator->SetupAttachment(GetRootComponent());
	OtherPositionLocator->SetChildActorClass(AActor::StaticClass());

	TravelTime = 1.f;
}

// Called when the game starts or when spawned
void AMapElementMover::BeginPlay()
{
	Super::BeginPlay();
	TravelStateNormalized = 0.f;
	
}

// Called every frame
void AMapElementMover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsReturning)
	{
		TravelStateNormalized -= DeltaTime / TravelTime;
		if (TravelStateNormalized <= 0.f)
		{
			bIsReturning = false;
			TravelStateNormalized = 0.f;
		}
	}
	else
	{
		TravelStateNormalized += DeltaTime / TravelTime;
		if (TravelStateNormalized >= 1.f)
		{
			bIsReturning = true;
			TravelStateNormalized = 1.f;
		}
	}
	FVector travelVector = OtherPositionLocator->GetComponentLocation() - GetActorLocation();
	float travelVectorRatio = 0.f;
	if (MovementCurve != nullptr)
	{
		travelVectorRatio = MovementCurve->GetFloatValue(TravelStateNormalized);
	}
	else
	{
		travelVectorRatio = TravelStateNormalized;
	}
	if (MovedActor != nullptr)
	{
		MovedActor->SetActorLocation(GetActorLocation() + travelVector * travelVectorRatio);
	}
	
}

