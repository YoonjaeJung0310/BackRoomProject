// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Coin_Item.h"
#include "Small_Coin_Item.generated.h"

/**
 * 
 */
UCLASS()
class ALADINPROJECT_API ASmall_Coin_Item : public ACoin_Item
{
	GENERATED_BODY()
public:
	ASmall_Coin_Item();

	virtual void ActivateItem(AActor* Activator) override;

};
