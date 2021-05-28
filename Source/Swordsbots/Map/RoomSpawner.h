// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <GameFramework/Actor.h>
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>

#include "RoomLevel.h"
#include "RoomSpawner.generated.h"

class ULevel;
class ULevelStreamingDynamic;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRoomSpawnedDelegate, ARoomLevel*, SpawnedRoom);

UCLASS()
class SWORDSBOTS_API ARoomSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomSpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateRoom();

	FORCEINLINE bool SpawningDone() { return SpawnedRoomLevel != nullptr; }
	FORCEINLINE bool RoomActivated() { return bRoomActivated; }

	UPROPERTY(BlueprintAssignable)
	FRoomSpawnedDelegate RoomSpawnedDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Spawning", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RoomPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Spawning", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RoomPreviewMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Spawning", meta = (AllowPrivateAccess = "true"))
	TArray<TAssetPtr<UWorld>> PossibleRoomTypes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ULevelStreamingDynamic* SpawnedRoomLevel;

	UFUNCTION()
	void OnTriggerVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRoomLoaded();

	bool bRoomActivated;
};
