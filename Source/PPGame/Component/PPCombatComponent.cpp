// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCombatComponent.h"

#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
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

void UPPCombatComponent::OnRep_EquippedWeapon(APPWeapon* OldEquippedWeapon)
{
	if (PPCharacter->GetLocalRole() != ROLE_SimulatedProxy)
	{
		PPCharacter->bUseControllerRotationYaw = EquippedWeapon != nullptr;
		PPCharacter->GetCharacterMovement()->bOrientRotationToMovement = EquippedWeapon == nullptr;
	}
}

void UPPCombatComponent::OnRep_Aiming(bool OldbAiming)
{
	PPCharacter->GetCharacterMovement()->MaxWalkSpeed = bAiming ? MaxAimWalkSpeed : MaxBaseWalkSpeed;
}

void UPPCombatComponent::ServerAim_Implementation(bool bAim)
{
	Aim(bAim);
}

void UPPCombatComponent::ServerEquipWeapon_Implementation(APPWeapon* Weapon2Equip)
{
	EquipWeapon(Weapon2Equip);
}

