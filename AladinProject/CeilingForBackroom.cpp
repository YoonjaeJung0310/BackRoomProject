// Fill out your copyright notice in the Description page of Project Settings.


#include "CeilingForBackroom.h"
#include "Components/BoxComponent.h"

// Sets default values
ACeilingForBackroom::ACeilingForBackroom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = false;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SpawningBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SpawningBox->SetupAttachment(StaticMesh);
	ItemDataTable = nullptr;
}
AActor* ACeilingForBackroom::SpawnRandomItem()
{
	if (FItemSpawn* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}
	return nullptr;
}

FItemSpawn* ACeilingForBackroom::GetRandomItem() const
{
	if (!ItemDataTable) return nullptr;

	TArray<FItemSpawn*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (const FItemSpawn* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}
	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;
	//0.0~1.0
	//랜덤값 0.7
	//Item A 0.5
	//Item B 0.8
	//Item C 1.0
	//그러면 아이템 B가 나옴 누적 랜덤 값

	for (FItemSpawn* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue < AccumulateChance)
		{
			return Row;
		}
	}
	return nullptr;
}
FVector ACeilingForBackroom::GetRandomPointInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	//크기가(200,100,50), Scale이 (2,1,1)이면 -> (400,100,50) 반지름 값을 가져옴
	//중심->끝까지의 거리
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}
AActor* ACeilingForBackroom::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
	);
	return SpawnedActor;
}
