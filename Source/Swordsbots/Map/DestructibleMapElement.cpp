// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleMapElement.h"
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Sound/SoundCue.h>
#include "../GameInstance/SoundGameSubsystem.h"

// Sets default values
ADestructibleMapElement::ADestructibleMapElement()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	SetRootComponent(MeshComponent);
}

float ADestructibleMapElement::TakeDamage(float Amount, const FDamageEvent& DamageEvent, AController* DamageInstigator, AActor* Source)
{
	TotalHealth -= Amount;

	FVector fxPoint;
	MeshComponent->GetClosestPointOnCollision(Source->GetActorLocation(), fxPoint);
	// Spawn Damage particle
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamagedParticles, fxPoint, FQuat::Identity.Rotator(), true);

	if (TotalHealth <= 0) 
	{
		// Play Destroyed sound
		GetGameInstance()->GetSubsystem<USoundGameSubsystem>()->PlayFXSound(DamagedSounds[UKismetMathLibrary::RandomIntegerInRange(0, DamagedSounds.Num() - 1)], fxPoint);

		OnDestroyedDelegate.Broadcast();
		Destroy(); // TODO : Allow to swap to a different mesh instead, and spawn a particle effect.
	}
	else
	{
		// Play Damaged sound
		GetGameInstance()->GetSubsystem<USoundGameSubsystem>()->PlayFXSound(DestroyedSound, fxPoint);
		// TODO : Add shake component to add visual shake when damaging the element
	}

	return Amount;
}

