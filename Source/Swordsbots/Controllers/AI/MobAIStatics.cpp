// Fill out your copyright notice in the Description page of Project Settings.


#include "MobAIStatics.h"
#include "../../Pawns/Mob_Base.h"
#include <Kismet/KismetSystemLibrary.h>

AMob_Base* MobAIStatics::FindClosestTargetFor(AMob_Base* Mob, float Range)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>();
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> actorsToIgnore = TArray<AActor*>();
	actorsToIgnore.Add(Mob);

	TArray<AActor*> actorsScanResults = TArray<AActor*>();
	TArray<AMob_Base*> potentialTargets = TArray<AMob_Base*>();
	AMob_Base* target = nullptr;
	if (UKismetSystemLibrary::SphereOverlapActors(Mob->GetWorld(), Mob->GetActorLocation(), Range, objectTypes, nullptr, actorsToIgnore, actorsScanResults))
	{
		for (AActor* result : actorsScanResults)
		{
			AMob_Base* mob = Cast<AMob_Base>(result);
			if (mob != nullptr)
			{
				if (mob->IsAlive() && Mob->IsEnemyOf(mob))
				{
					potentialTargets.Add(mob);
				}
			}
		}
		// Among all potential targets found, target the closest one.
		
		float shortestSquaredDist = 0.f;
		for (int potentialTargetID = 0; potentialTargetID < potentialTargets.Num(); potentialTargetID++)
		{
			float squaredDist = (Mob->GetActorLocation() - potentialTargets[potentialTargetID]->GetActorLocation()).SizeSquared();
			if (target == nullptr || squaredDist < shortestSquaredDist)
			{
				target = potentialTargets[potentialTargetID];
				shortestSquaredDist = squaredDist;
			}
		}
	}

	return target;
}