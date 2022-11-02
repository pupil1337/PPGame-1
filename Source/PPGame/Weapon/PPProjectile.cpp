// Fill out your copyright notice in the Description page of Project Settings.


#include "PPProjectile.h"
#include "Components/BoxComponent.h"

APPProjectile::APPProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
}

void APPProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void APPProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

