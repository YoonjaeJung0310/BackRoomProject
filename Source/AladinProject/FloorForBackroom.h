// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorForBackroom.generated.h"

class UBoxComponent;

UCLASS()
class ALADINPROJECT_API AFloorForBackroom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorForBackroom();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Component")
	UBoxComponent* Collision;


};
