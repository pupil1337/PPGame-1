// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCombatComponent.h"

#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PPGame/GameFramework/PPCharacter.h"
#include "PPGame/Weapon/PPWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

UPPCombatComponent::UPPCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPPCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UPPCombatComponent, EquippedWeapon, COND_None);
	DOREPLIFETIME_CONDITION(UPPCombatComponent, bAiming, COND_SkipOwner);
}

void UPPCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPPCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);
}

void UPPCombatComponent::EquipWeapon(APPWeapon* Weapon2Equip)
{
	if (PPCharacter && Weapon2Equip)
	{
		if (PPCharacter->HasAuthority())
		{
			APPWeapon* OldEquippedWeapon = EquippedWeapon;
			EquippedWeapon = Weapon2Equip;
			EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
			if (const USkeletalMeshSocket* WeaponSocket = PPCharacter->GetMesh()->GetSocketByName(FName("RightHandSocket")))
			{
				WeaponSocket->AttachActor(EquippedWeapon, PPCharacter->GetMesh());
			}
			EquippedWeapon->SetOwner(PPCharacter);
			OnRep_EquippedWeapon(OldEquippedWeapon);
		}
		else
		{
			ServerEquipWeapon(Weapon2Equip);
		}
	}
}

void UPPCombatComponent::ServerEquipWeapon_Implementation(APPWeapon* Weapon2Equip)
{
	EquipWeapon(Weapon2Equip);
}

void UPPCombatComponent::OnRep_EquippedWeapon(APPWeapon* OldEquippedWeapon)
{
	if (PPCharacter->GetLocalRole() != ROLE_SimulatedProxy)
	{
		PPCharacter->bUseControllerRotationYaw = EquippedWeapon != nullptr;
		PPCharacter->GetCharacterMovement()->bOrientRotationToMovement = EquippedWeapon == nullptr;
	}
}

void UPPCombatComponent::Aim(bool bAim)
{
	bool OldbAiming = bAiming;
	bAiming = bAim;
	OnRep_Aiming(OldbAiming);
	
	if (!PPCharacter->HasAuthority())
	{
		ServerAim(bAim);
	}
}

void UPPCombatComponent::ServerAim_Implementation(bool bAim)
{
	Aim(bAim);
}

void UPPCombatComponent::OnRep_Aiming(bool OldbAiming)
{
	PPCharacter->GetCharacterMovement()->MaxWalkSpeed = bAiming ? MaxAimWalkSpeed : MaxBaseWalkSpeed;
}

void UPPCombatComponent::Fire(bool bFire)
{
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		OnFire(bFire);
	}
	ServerFire(bFire);
}

void UPPCombatComponent::OnFire(bool bFire)
{
	if (PPCharacter && EquippedWeapon)
	{
		if (bFire)
		{
			PPCharacter->PlayFireMontage(bAiming);
			EquippedWeapon->Fire(HitTarget);
		}
	}
}

void UPPCombatComponent::ServerFire_Implementation(bool bFire)
{
	MulticastFire(bFire);
}

void UPPCombatComponent::MulticastFire_Implementation(bool bFire)
{
	if (GetOwnerRole() != ROLE_AutonomousProxy)
	{
		OnFire(bFire);
	}
}

void UPPCombatComponent::TraceUnderCrosshairs(FHitResult& HitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector WorldPosition, WorldDirection;
	bool ScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		ViewportSize / 2.0f,
		WorldPosition,
		WorldDirection
	);
	if (ScreenToWorld)
	{
		FVector Start = WorldPosition;
		FVector End = Start + WorldDirection*10000.0f;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this->GetOwner());
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility,
			QueryParams
		);
		if (!HitResult.bBlockingHit)
		{
			HitResult.ImpactPoint = End;
		}
		HitTarget = HitResult.ImpactPoint;
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 10, FColor::Red);
	}
}

