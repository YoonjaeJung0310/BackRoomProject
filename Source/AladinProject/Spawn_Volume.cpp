// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawn_Volume.h"
#include "Components/BoxComponent.h"

ASpawn_Volume::ASpawn_Volume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawiningBox"));
	SpawningBox->SetupAttachment(Scene);
	ItemDataTable = nullptr;

}
AActor* ASpawn_Volume::SpawnRandomItem()
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

FItemSpawn* ASpawn_Volume::GetRandomItem() const
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
FVector ASpawn_Volume::GetRandomPointInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	//크기가(200,100,50), Scale이 (2,1,1)이면 -> (400,100,50) 반지름 값을 가져옴
	//중심->끝까지의 거리
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		0.0f);
	
}
//FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z
AActor* ASpawn_Volume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;
	AActor* SpawnedActor=GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
	);
	return SpawnedActor;
}

