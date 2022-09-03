// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PPCombatComponent.generated.h"

class APPCharacter;
class APPWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PPGAME_API UPPCombatComponent : public UActorComponent
{
	GENERATED_BODY()
	friend APPCharacter;
	
public:
	UPPCombatComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 装备武器
	virtual void EquipWeapon(APPWeapon* Weapon2Equip);
	// 瞄准
	virtual void Aim(bool bAim);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnRep_EquippedWeapon(APPWeapon* OldEquippedWeapon);
	UFUNCTION()
	virtual void OnRep_Aiming(bool OldbAiming);
	
	UFUNCTION(Server, Reliable)
	virtual void ServerEquipWeapon(APPWeapon* Weapon2Equip);
	UFUNCTION(Server, Reliable)
	virtual void ServerAim(bool bAim);

private:
	UPROPERTY()
	APPCharacter* PPCharacter;
	
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	APPWeapon* EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_Aiming)
	bool bAiming;
};
