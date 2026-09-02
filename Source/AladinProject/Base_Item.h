// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_Interface.h"
#include "Base_Item.generated.h"

class USphereComponent;

UCLASS()
class ALADINPROJECT_API ABase_Item : public AActor,  public IItem_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABase_Item();

protected:
	virtual void OnItemOverLap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	virtual void OnItemEndOverLap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	) override;
	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;//이걸 Fstring 말고  FName으로 하는 이유는 훨씬 속도 빠르고 메모리 절약이 가능!
	virtual void DestroyItem();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Component")
	USphereComponent* Collision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* PickupParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* PickupSound;

	
};
