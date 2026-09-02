// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Item.h"
#include "DisguiseHealingItem.generated.h"

/**
 * 
 */
UCLASS()
class ALADINPROJECT_API ADisguiseHealingItem : public ABase_Item
{
	GENERATED_BODY()
	public:
	ADisguiseHealingItem();
	virtual void ActivateItem(AActor* Activator) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float DebuffPercent;
	

	// 움직임 불가 지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float DisableMoveDuration;
};