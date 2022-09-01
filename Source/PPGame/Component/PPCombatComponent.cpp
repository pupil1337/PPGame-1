// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCombatComponent.h"

#include "Engine/SkeletalMeshSocket.h"
#include "PPGame/GameFramework/PPCharacter.h"
#include "PPGame/Weapon/PPWeapon.h"
#include "Net/UnrealNetwork.h"

UPPCombatComponent::UPPCombatComponent()
{
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

void UPPCombatComponent::EquipWeapon(APPWeapon* Weapon2Equip)
{
	if (PPCharacter && Weapon2Equip)
	{
		if (PPCharacter->HasAuthority())
		{
			EquippedWeapon = Weapon2Equip;
			EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
			if (const USkeletalMeshSocket* WeaponSocket = PPCharacter->GetMesh()->GetSocketByName(FName("RightHandSocket")))
			{
				WeaponSocket->AttachActor(EquippedWeapon, PPCharacter->GetMesh());
			}
			EquippedWeapon->SetOwner(PPCharacter);
		}
		else
		{
			ServerEquipWeapon(Weapon2Equip);
		}
	}
}

void UPPCombatComponent::Aim(bool bAim)
{
	bAiming = bAim;
	if (!PPCharacter->HasAuthority())
	{
		ServerAim(bAim);
	}
}

void UPPCombatComponent::ServerAim_Implementation(bool bAim)
{
	Aim(bAim);
}

void UPPCombatComponent::ServerEquipWeapon_Implementation(APPWeapon* Weapon2Equip)
{
	EquipWeapon(Weapon2Equip);
}

