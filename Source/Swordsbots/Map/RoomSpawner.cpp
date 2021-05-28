// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomSpawner.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/LevelStreaming.h>
#include <Engine/LevelStreamingDynamic.h>

// Sets default values
ARoomSpawner::ARoomSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RoomPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Room Position"));
	SetRootComponent(RoomPosition);

	RoomPreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Room Preview Mesh"));
	RoomPreviewMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ARoomSpawner::BeginPlay()
{
	Super::BeginPlay();

	GenerateRoom();
}

// Called every frame
void ARoomSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoomSpawner::GenerateRoom()
{
	// TODO : Consider moving all level streaming code to a single place.
	// Choose a Room level class at random, level stream it into existence at our position then destroy ourselves.

	FString RoomPath = PossibleRoomTypes[UKismetMathLibrary::RandomIntegerInRange(0, PossibleRoomTypes.Num() - 1)].GetLongPackageName();

	bool loadSuccessful;
	SpawnedRoomLevel = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), RoomPath, GetActorLocation(), GetActorRotation(), loadSuccessful);
	SpawnedRoomLevel->bShouldBlockOnLoad = true;
	SpawnedRoomLevel->SetShouldBeLoaded(true);
	if (loadSuccessful)
	{
		SpawnedRoomLevel->OnLevelLoaded.AddDynamic(this, &ARoomSpawner::OnRoomLoaded);
		RoomPreviewMesh->DestroyComponent();
	}
}

void ARoomSpawner::OnTriggerVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*if (SpawningDone() == false || RoomActivated()) return;

	APawn* overlappedPawn = Cast<APawn>(OtherActor);
	if (overlappedPawn != nullptr && overlappedPawn->GetController() != nullptr && overlappedPawn->GetController()->IsPlayerController())
	{
		ALevelScriptActor* levelScriptActor = SpawnedRoomLevel->GetLevelScriptActor();
		ARoomLevel* roomLevel = Cast<ARoomLevel>(levelScriptActor);
		roomLevel->OnRoomActivated();
		bRoomActivated = true;
	}*/
}

void ARoomSpawner::OnRoomLoaded()
{
	ALevelScriptActor* levelScriptActor = SpawnedRoomLevel->GetLevelScriptActor();
	ARoomLevel* roomLevel = Cast<ARoomLevel>(levelScriptActor);
	roomLevel->OnRoomSpawned();

	roomLevel->RoomTransform = SpawnedRoomLevel->LevelTransform;

	RoomSpawnedDelegate.Broadcast(roomLevel);
}

