// Fill out your copyright notice in the Description page of Project Settings.


#include "PPProjectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

APPProjectile::APPProjectile()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

void APPProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ProjectileMovementComponent->OnProjectileStop.AddUniqueDynamic(this, &ThisClass::OnProjectileStop);
}	

void APPProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() != ENetMode::NM_DedicatedServer)
	{
		if (TracePS)
		{
			UGameplayStatics::SpawnEmitterAttached(TracePS, RootComponent);
		}
	}
}

void APPProjectile::OnProjectileStop(const FHitResult& ImpactResult)
{
	if (GetNetMode() != ENetMode::NM_DedicatedServer)
	{
		if (ImpactPS)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactPS, ImpactResult.ImpactPoint, ImpactResult.ImpactNormal.Rotation());
		}
		if (ImpactSC)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, ImpactSC, ImpactResult.ImpactPoint, ImpactResult.ImpactNormal.Rotation());
		}
	}
	Destroy();
}
