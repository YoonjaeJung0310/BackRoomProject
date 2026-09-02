// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Item_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItem_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALADINPROJECT_API IItem_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public://순수 가상 함수를 밑에서(구현할 아이템 지뢰,힐링,코인)
	//힐링,코인-즉시 발동형-오버랩
	//지뢰-범위 내에 오버랩-발동이 5초 뒤 폭발-오버랩-데미지
	UFUNCTION()
	virtual void OnItemOverLap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;
	UFUNCTION()
	virtual void OnItemEndOverLap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;
	virtual void ActivateItem(AActor* Activator) = 0;
	virtual FName GetItemType() const = 0;//이걸 Fstring 말고  FName으로 하는 이유는 훨씬 속도 빠르고 메모리 절약이 가능!
};

