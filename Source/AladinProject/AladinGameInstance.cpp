// Fill out your copyright notice in the Description page of Project Settings.


#include "AladinGameInstance.h"

UAladinGameInstance::UAladinGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UAladinGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}
