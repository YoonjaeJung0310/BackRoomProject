// Fill out your copyright notice in the Description page of Project Settings.


#include "Big_Coin_Item.h"

ABig_Coin_Item::ABig_Coin_Item()
{
	PointValue = 50;
	ItemType = "BigCoin";
}

void ABig_Coin_Item::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}