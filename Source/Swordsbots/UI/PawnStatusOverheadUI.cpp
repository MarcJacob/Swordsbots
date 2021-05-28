// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnStatusOverheadUI.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UPawnStatusOverheadUI::UPawnStatusOverheadUI()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	DisplayDistance = 500.f;
	// ...
}


// Called when the game starts
void UPawnStatusOverheadUI::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetClass != nullptr)
	{
		SpawnedWidget = CreateWidget<UPawnOverheadUserWidget, APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0), WidgetClass.Get());
		SpawnedWidget->AddToViewport();
		SpawnedWidget->SetVisibility(ESlateVisibility::Collapsed);
		APawn* ownerPawn = Cast<APawn>(GetOwner());
		SpawnedWidget->AssignPawn(ownerPawn);
	}

	
}


// Called every frame
void UPawnStatusOverheadUI::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (SpawnedWidget == nullptr || !IsValid(SpawnedWidget)) return;

	float distanceToPlayer = FVector::Dist(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation(), GetComponentLocation());
	if (distanceToPlayer < DisplayDistance)
	{
		// Update widget position
		FVector2D widgetScreenPos;
		if (UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0), GetComponentLocation(), widgetScreenPos, false))
		{
			if (SpawnedWidget->Visibility == ESlateVisibility::Collapsed)
			{
				SpawnedWidget->SetVisibility(ESlateVisibility::Visible);
			}
			
			SpawnedWidget->SetPositionInViewport(widgetScreenPos);
		}
		else if (SpawnedWidget->Visibility == ESlateVisibility::Visible)
		{
			SpawnedWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else if (SpawnedWidget->Visibility == ESlateVisibility::Visible)
	{
		SpawnedWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}

