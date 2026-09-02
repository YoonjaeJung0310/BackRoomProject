// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "AladinGameState.generated.h"



UCLASS()
class ALADINPROJECT_API AAladinGameState : public AGameState
{
	GENERATED_BODY()
public:
	AAladinGameState();

	virtual void BeginPlay();


	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	float LevelDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 ItemToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 ItemToSpawn2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TArray<FName> LevelMapNames;

	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;
	
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	
	FTimerHandle SpawnTimerHandle;
	FTimerHandle FallingHandsTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cycle")
	int32 SpawnCycle=20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cycle")
	float SpawnCycle2 = 0.2;

	void StartLevel();

	void OnLevelTimeUp();
	void OnCoinCollected();
	void EndLevel();
	void UpdateHUD();
	void SpawnItems();
	void SpawnFallingHands();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveCount")
	int32 WaveCount = 1;
};
