// Fill out your copyright notice in the Description page of Project Settings.


#include "Mine_Item.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMine_Item::AMine_Item()
{
	ExplosionDamage = 20;
	ExplosionDelay = 1;
	ExplosionRadius = 300;
	ItemType = "Mine";
	bHasExploded = false;

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
}

void AMine_Item::ActivateItem(AActor* Activator)
{
	if (bHasExploded) return;
	//게임 월드-> 타임 매니저(타이머 핸들러들을 관리)
	//타이머 핸들러 각자의 시계
	Super::ActivateItem(Activator);

	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimerHandle,
		this,
		&AMine_Item::Explode,
		ExplosionDelay,
		false
	);
	bHasExploded = true;
}

void AMine_Item::Explode()
{

	UParticleSystemComponent* Particle = nullptr;
	if (ExplosionParticle)
	{
		Particle=UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticle,
			GetActorLocation(),
			GetActorRotation(),
			false);
	}
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplosionSound,
			GetActorLocation());
	}
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,//받는 주체(엑터)
				ExplosionDamage,//받는 데미지
				nullptr,//데미지를 유발한 주체(엑터), 여기서는 아이템이니 nullptr로
				this,//데미지를 입힌 엑터
				UDamageType::StaticClass()//데미지의 유형, 여기서는 일반적인 유형으로 씀
			);
	
		}
	}
	DestroyItem();//아이템 소멸
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

