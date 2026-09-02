// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMonsters::AMonsters()
{
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionResponseToAllChannels(ECR_Block);
	Collision->SetNotifyRigidBodyCollision(true);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StaticMesh"));
	SkeletalMesh->SetupAttachment(Collision);

	MoveDirection = 1;
	RandomMoveSpeed = 1.0f;

	Collision->OnComponentHit.AddDynamic(
		this,
		&AMonsters::OnMonstersHit
	);
}

void AMonsters::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();

	CurrentLocation.X += RandomMoveSpeed * DeltaTime * MoveDirection;
	CurrentLocation.Y += RandomMoveSpeed * DeltaTime * MoveDirection;

	SetActorLocation(CurrentLocation, true);
}

void AMonsters::OnMonstersHit(
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
	else if (OtherActor)
	{
		MoveDirection*=-1;
	}
	

}

