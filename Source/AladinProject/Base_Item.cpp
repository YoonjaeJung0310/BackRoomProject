// Fill out your copyright notice in the Description page of Project Settings.
#include "Base_Item.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABase_Item::ABase_Item()
{

	PrimaryActorTick.bCanEverTick = false;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	// 이벤트 바인딩
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABase_Item::OnItemOverLap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABase_Item::OnItemEndOverLap);
}

void  ABase_Item::OnItemOverLap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		ActivateItem(OtherActor);
	}
}
void  ABase_Item::OnItemEndOverLap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}
void  ABase_Item::ActivateItem(AActor* Activator)
{
	UParticleSystemComponent* Particle=nullptr;
	if (PickupParticle)
	{
		Particle=UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		PickupParticle,
		GetActorLocation(),
		GetActorRotation(),
		true
		);//지정된 곳에 파티를 소환
	}
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			PickupSound,
			GetActorLocation()
        ); // 지정된 곳에 Sound
	}
	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[Particle]()
			{
				Particle->DestroyComponent();
			},
			2.0f,
			false
		);
	}
}
FName ABase_Item::GetItemType() const
{
	return ItemType;
}

void ABase_Item::DestroyItem()
{
	Destroy();
	
}
