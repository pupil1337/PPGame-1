// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPWeapon.h"
#include "PPProjectileWeapon.generated.h"

class APPProjectile;

/**
 * 
 */
UCLASS()
class PPGAME_API APPProjectileWeapon : public APPWeapon
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APPProjectile> ProjectileClass;
	
public:
	APPProjectileWeapon();

public:
	virtual void Fire(const FVector& HitTarget) override;
};
