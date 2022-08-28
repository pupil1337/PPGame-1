// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeapon.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "PPGame/GameFramework/PPCharacter.h"

APPWeapon::APPWeapon()
{
	bReplicates = true;

	// WeaponMesh
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// AreaSphere
	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaShpereComponent"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentBeginOverlap);
		AreaSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentEndOverlap);
	}

	// PickTipComponent
	PickTipComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidgetComponent"));
	PickTipComponent->SetupAttachment(RootComponent);
	PickTipComponent->SetVisibility(false);
}

void APPWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void APPWeapon::SetPickupTipVisibility(bool bVisibility)
{
	PickTipComponent->SetVisibility(bVisibility);
}

void APPWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APPCharacter* PPCharacter = Cast<APPCharacter>(OtherActor))
	{
		PPCharacter->SetOverlapWeapon(this);
	}
}

void APPWeapon::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APPCharacter* PPCharacter = Cast<APPCharacter>(OtherActor))
	{
		PPCharacter->SetOverlapWeapon(nullptr);
	}
}

