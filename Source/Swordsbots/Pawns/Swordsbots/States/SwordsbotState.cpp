// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordsbotState.h"

void USwordsbotState::AssignControlledMob(AMob_Base* Target)
{
	Super::AssignControlledMob(Target);
	TargetSwordsbot = Cast<ASwordsbot>(Target);
	if (TargetSwordsbot == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR : Attempted to run a Swordsbot state on a non Swordsbot mob !"));
		EndState();
	}
}