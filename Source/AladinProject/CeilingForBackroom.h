// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_Spawn_Row.h"
#include "CeilingForBackroom.generated.h"

class UBoxComponent;
UCLASS()
class ALADINPROJECT_API ACeilingForBackroom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACeilingForBackroom();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* StaticMesh;
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomItem();


	FItemSpawn* GetRandomItem() const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
	FVector GetRandomPointInVolume() const;


};
