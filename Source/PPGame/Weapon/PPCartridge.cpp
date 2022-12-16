// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCartridge.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

APPCartridge::APPCartridge()
	: bSound(true)
{
	InitialLifeSpan = 10.0f;
	CartridgeStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CartridgeStaticMesh"));
	SetRootComponent(CartridgeStaticMesh);
	CartridgeStaticMesh->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	CartridgeStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CartridgeStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CartridgeStaticMesh->SetSimulatePhysics(true);
	CartridgeStaticMesh->SetEnableGravity(true);
	CartridgeStaticMesh->SetNotifyRigidBodyCollision(true);
}

void APPCartridge::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	CartridgeStaticMesh->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnHit);
}

void APPCartridge::Eject(FVector InitialVel)
{
	FVector EjectVel = UKismetMathLibrary::TransformDirection(GetActorTransform(), EjectVelocity);
	CartridgeStaticMesh->SetPhysicsLinearVelocity(InitialVel + EjectVel);
}

void APPCartridge::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         FVector NormalImpulse, const FHitResult& Hit)
{
	if (bSound)
	{
		UKismetSystemLibrary::PrintString(this, "Cartridge Sound~");
	}
	bSound = false;
}

