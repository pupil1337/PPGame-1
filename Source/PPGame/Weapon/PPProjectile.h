// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPProjectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class USoundCue;

UCLASS()
class PPGAME_API APPProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, DisplayName = "Box碰撞组件")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleDefaultsOnly, DisplayName = "Projectile移动组件")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Trace特效")
	TObjectPtr<UParticleSystem> TracePS;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Impact特效")
	TObjectPtr<UParticleSystem> ImpactPS;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Impace音效")
	TObjectPtr<USoundCue> ImpactSC;
public:
	APPProjectile();
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnProjectileStop(const FHitResult& ImpactResult);
};
