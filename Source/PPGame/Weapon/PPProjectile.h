// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPProjectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class PPGAME_API APPProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, DisplayName = "Box碰撞组件")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleDefaultsOnly, DisplayName = "Projectile移动组件")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
public:
	APPProjectile();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

};
