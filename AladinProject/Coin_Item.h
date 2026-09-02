// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Item.h"
#include "Coin_Item.generated.h"

UCLASS()
class ALADINPROJECT_API ACoin_Item : public ABase_Item
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoin_Item();

protected:
	UPROPERTY(EditANywhere, BlueprintReadWrite, Category = "Coin")
	int32 PointValue;

	virtual void ActivateItem(AActor* Activator) override;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatingPlatform|Properties")
	//회전 속도를 나태나느 변수(초당 도 단위)
	float RotationSpeed;
};


