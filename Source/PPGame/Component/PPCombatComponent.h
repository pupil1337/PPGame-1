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

	virtual void EquipWeapon(APPWeapon* Weapon2Equip);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	APPCharacter* PPCharacter;
	
	UPROPERTY(Replicated)
	APPWeapon* EquippedWeapon;
};
