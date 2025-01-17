﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeapon.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "PPGame/GameFramework/PPCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "PPGame/Weapon/PPCartridge.h"

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
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// PickTipComponent
	PickTipComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidgetComponent"));
	PickTipComponent->SetupAttachment(RootComponent);
	PickTipComponent->SetVisibility(false);
}

void APPWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APPWeapon, WeaponState, COND_None)
}

void APPWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AreaSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentBeginOverlap);
		AreaSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentEndOverlap);
	}
}

void APPWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void APPWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	switch (State)
	{
	case EWeaponState::EWS_Equipped:
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EWeaponState::EWS_Dropped:
		break;
	default:
		break;
	}
}

void APPWeapon::OnRep_WeaponState(EWeaponState OldWeaponState)
{
	UE_LOG(LogTemp, Log, TEXT("WeaponState Replicated!"))
}

void APPWeapon::SetPickupTipVisibility(bool bVisibility)
{
	PickTipComponent->SetVisibility(bVisibility);
}

void APPWeapon::Fire(const FVector& HitTarget)
{
	if (FireAnim)
	{
		WeaponMesh->PlayAnimation(FireAnim, false);	
	}
	if (GetNetMode() != NM_DedicatedServer)
	{
		if (CartridgeClass)
		{
			if (const USkeletalMeshSocket* AmmoEjectSocket = WeaponMesh->GetSocketByName(FName("AmmoEject")))
			{
				APPCartridge* tCartridge = GetWorld()->SpawnActor<APPCartridge>(CartridgeClass, AmmoEjectSocket->GetSocketTransform(WeaponMesh));
				tCartridge->Eject(GetOwner()->GetVelocity());
			}
		}
	}
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

