// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameplayCameraComponent.generated.h"

class UPostProcessComponent;

/**
 * 
 */
UCLASS(ClassGroup = (Camera), meta = (BlueprintSpawnableComponent))
class SWORDSBOTS_API UGameplayCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:

	UGameplayCameraComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	UFUNCTION()
	void OnPlayerSwordsbotHurt(float Damage);
	UFUNCTION()
		void OnPlayerSwordsbotDied();

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostProcessing | Animation", meta = (AllowPrivateAccess = "true"))
	float HurtPostProcessingAnimationTime;

	UPROPERTY(Instanced, VisibleAnywhere, BlueprintReadWrite, Category = "Post Processing | Data", meta = (AllowPrivateAccess = "true"))
	UPostProcessComponent* HurtPostProcessComponent;

	UPROPERTY(Instanced, VisibleAnywhere, BlueprintReadWrite, Category = "Post Processing | Data", meta = (AllowPrivateAccess = "true"))
	UPostProcessComponent* DeadPostProcessComponent;

	float NormalizedHurtPostProcessingWeight;
	float NormalizedDeadPostProcessingWeight;

};
