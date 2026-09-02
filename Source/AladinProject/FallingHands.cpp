// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingHands.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AFallingHands::AFallingHands()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionResponseToAllChannels(ECR_Block);
	Collision->SetNotifyRigidBodyCollision(true);

	SkeletalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(Collision);


	Collision->OnComponentHit.AddDynamic(
		this,
		&AFallingHands::OnMonstersHit
	);
}

void AFallingHands::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
VerticalVelocity += Gravity * DeltaTime;

		// 중력은 항상 월드 Z축 아래 방향으로 적용
		AddActorWorldOffset(
			FVector(0.0f, 0.0f, VerticalVelocity * DeltaTime),
			true
		);
}

void AFallingHands::OnMonstersHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{

	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,//받는 주체(엑터)
			HitDamage,//받는 데미지
			nullptr,//데미지를 유발한 주체(엑터), 여기서는 아이템이니 nullptr로
			this,//데미지를 입힌 엑터
			UDamageType::StaticClass()//데미지의 유형, 여기서는 일반적인 유형으로 씀
		);
		Destroy();
		UParticleSystemComponent* Particle = nullptr;
		if (HitParticle)
		{
			Particle = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				HitParticle,
				GetActorLocation(),
				GetActorRotation(),
				false);
		}
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				HitSound,
				GetActorLocation());
		}
	}
	else if (OtherActor && OtherActor->ActorHasTag("Floor"))
	{
		Destroy();
	}


}
