// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPCartridge.generated.h"

class UStaticMeshComponent;

UCLASS()
class PPGAME_API APPCartridge : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> CartridgeStaticMesh;
	
public:
	APPCartridge();
	
protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

private:
	bool bSound : 1;
};
