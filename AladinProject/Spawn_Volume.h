// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item_Spawn_Row.h"
#include "GameFramework/Actor.h"
#include "Spawn_Volume.generated.h"

class UBoxComponent;

UCLASS()
class ALADINPROJECT_API ASpawn_Volume : public AActor
{
	GENERATED_BODY()
	
public:
	ASpawn_Volume();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UBoxComponent* Collision;
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomItem();


	FItemSpawn* GetRandomItem() const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
	FVector GetRandomPointInVolume() const;



	



};
