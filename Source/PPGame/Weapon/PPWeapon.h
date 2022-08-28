// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState: uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped State"),
	EWS_Dropped UMETA(DisplayName = "Dropped State"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX"),
};

class USphereComponent;
class UWidgetComponent;
class APPCharacter;

UCLASS()
class PPGAME_API APPWeapon : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> AreaSphere;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UWidgetComponent> PickTipComponent;
	
public:
	APPWeapon();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetWeaponState(EWeaponState State);

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetPickupTipVisibility(bool bVisibility);

protected:
	UFUNCTION()
	virtual void OnRep_WeaponState(EWeaponState OldWeaponState);

private:
	UPROPERTY(ReplicatedUsing = OnRep_WeaponState)
	EWeaponState WeaponState;
	
	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
