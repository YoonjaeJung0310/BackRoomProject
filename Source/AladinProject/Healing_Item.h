// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Item.h"
#include "Healing_Item.generated.h"

/**
 * 
 */
UCLASS()
class ALADINPROJECT_API AHealing_Item : public ABase_Item
{
	GENERATED_BODY()
public:
	AHealing_Item();
	virtual void ActivateItem(AActor* Activator) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 HealAmount;
};
