// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AladinGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALADINPROJECT_API UAladinGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UAladinGameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "GameData")
	int32 TotalScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category="GameData")
	void AddToScore(int32 Amount);
};
