// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monsters.generated.h"

class USphereComponent;
UCLASS()
class ALADINPROJECT_API AMonsters : public AActor
{
	GENERATED_BODY()
	
public:
	AMonsters();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 HitDamage;
	virtual void Tick(float DeltaTime);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* HitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* HitSound;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monsters")
	float RandomMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monsters")
	int32 MoveDirection;

	UFUNCTION()
	void OnMonstersHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);
};