// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin_Item.h"
#include "Engine/World.h"//월드를 가져오기
#include "AladinGameState.h"

ACoin_Item::ACoin_Item()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
	PrimaryActorTick.bCanEverTick = true;
}

void ACoin_Item::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (AAladinGameState* GameState = World->GetGameState<AAladinGameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}
		DestroyItem();
	}
}

void ACoin_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 필요시 디버깅용 코드 or 별도 조건으로

	//RotationSpeed가 0이 아니면 회전 처리
	if (!FMath::IsNearlyZero(RotationSpeed))
	{
		//초당 RoatationSpeed만큼, 한 프레임당 (Rotation*DeltaTime)만큼 회전
		AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));

	}
}