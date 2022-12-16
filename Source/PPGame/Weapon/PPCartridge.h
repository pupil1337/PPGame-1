// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPCartridge.generated.h"

class APPWeapon;
class UStaticMeshComponent;
class USoundCue;

UCLASS()
class PPGAME_API APPCartridge : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> CartridgeStaticMesh;

	UPROPERTY(EditDefaultsOnly)
	FVector EjectVelocity;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundCue> HitSC;
	
public:
	APPCartridge();
	virtual void PostInitializeComponents() override;
	void Eject(FVector InitialVel);
	
protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
};
