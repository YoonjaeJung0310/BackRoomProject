// Fill out your copyright notice in the Description page of Project Settings.


#include "Small_Coin_Item.h"

ASmall_Coin_Item::ASmall_Coin_Item()
{
	PointValue = 25;
	ItemType = "SmallCoin";
}

void ASmall_Coin_Item::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}