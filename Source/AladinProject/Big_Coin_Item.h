// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Coin_Item.h"
#include "Big_Coin_Item.generated.h"

/**
 * 
 */
UCLASS()
class ALADINPROJECT_API ABig_Coin_Item : public ACoin_Item
{
	GENERATED_BODY()
public:
	ABig_Coin_Item();

	virtual void ActivateItem(AActor* Activator) override;
};
