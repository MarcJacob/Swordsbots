// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleMapElement.generated.h"

class AController;
class AActor;
class UStaticMeshComponent;

class USoundCue;
class UParticleSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestroyedCallback);

/*
* Simple "Destructible" actor to use for static map elements that can be destroyed, sometimes with further effects
* defined using the OnDestroyed callback event. By default the object will simply disappear upon destruction.
* TODO : Add particle effect & optional swap mesh upon destruction.
*/
UCLASS()
class SWORDSBOTS_API ADestructibleMapElement : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructibleMapElement();

	float TakeDamage(float Amount, const FDamageEvent& DamageEvent, AController* Instigator, AActor* Source) override;

	UPROPERTY(BlueprintAssignable)
	FOnDestroyedCallback OnDestroyedDelegate;

private:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX", meta = (AllowPrivateAccess = "true"))
	TArray<USoundCue*> DamagedSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX", meta = (AllowPrivateAccess = "true"))
	USoundCue* DestroyedSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* DamagedParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	int TotalHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

};
