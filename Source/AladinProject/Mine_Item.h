// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Item.h"
#include "Mine_Item.generated.h"

/**
 * 
 */
UCLASS()
class ALADINPROJECT_API AMine_Item : public ABase_Item
{
	GENERATED_BODY()
public:
	AMine_Item();
	USphereComponent* ExplosionCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ExplosionDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* ExplosionParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* ExplosionSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ExplosionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ExplosionDamage;
	virtual void ActivateItem(AActor* Acticator) override;
	FTimerHandle ExplosionTimerHandle;
	void Explode();
	bool bHasExploded;
};
