// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include <Blueprint/UserWidget.h>
#include "PawnOverheadUserWidget.h"
#include "PawnStatusOverheadUI.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
/// <summary>
/// Allows a Pawn to position and activate (when close enough to the player) a UI Widget displaying information related to them.
/// This is only for placement and parenting purposes - the Widget itself handles reading as much as it can from the Pawn
/// and updating itself.
/// </summary>
class SWORDSBOTS_API UPawnStatusOverheadUI : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPawnStatusOverheadUI();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overhead UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPawnOverheadUserWidget> WidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overhead UI", meta = (AllowPrivateAccess = "true"))
	float DisplayDistance;

	UPROPERTY()
	UPawnOverheadUserWidget* SpawnedWidget;
};
