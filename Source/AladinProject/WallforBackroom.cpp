// Fill out your copyright notice in the Description page of Project Settings.


#include "WallforBackroom.h"
#include "components/BoxComponent.h"


DEFINE_LOG_CATEGORY(LogTemplateActor);
// Sets default values
AWallforBackroom::AWallforBackroom()
{

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	// 이벤트 바인딩
	//Collision->OnComponentBeginOverlap.AddDynamic(this, &AWallforBackroom::OnItemOverLap);

}

