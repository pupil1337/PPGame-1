// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PPGame/PPGameTypes/TurningInPlace.h"
#include "PPCharacter.generated.h"

class UWidgetComponent;
class UPPCombatComponent;
class UInputMappingContext;
class UInputAction;
class APPWeapon;
class UAnimMontage;

UCLASS()
class PPGAME_API APPCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> PlayerNameComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPPCombatComponent> CombatComp;

	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<UAnimMontage> FireWeaponMontage;
	
	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	TObjectPtr<UInputMappingContext> CharacterIMC;
	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_Jump;
	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_Pickup;
	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_CrouchStart;
	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_CrouchEnd;
	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_AimStart;
	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_AimEnd;
	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_FireStart;
	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_FireEnd;
	
public:
	APPCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PostInitializeComponents() override;
	
	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	virtual void OnRep_PlayerState() override;

protected:
	virtual void BeginPlay() override;

	void AimOffset(float DeltaTime);

public:
	/** 显示角色头顶Steam昵称 */
	void SetOverheadPlayerName();
	
	/** 设置新的可拾取武器 */
	void SetOverlapWeapon(APPWeapon* NewOverlapWeapon);
	UFUNCTION()
	void OnRep_OverlapWeapon(APPWeapon* OldOverlapWeapon);

	/** 播放开火动画 */
	void PlayFireMontage(bool bAiming);

	/** 是否装备武器 */
	bool GetIsEquipWeapon() const;
	/** 是否在瞄准 */
	bool GetIsAiming() const;
	/** 获取AO_Yaw*/
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	/** 获取AO_Pitch*/
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	/** 获取当前武器 */
	APPWeapon* GetEquippedWeapon() const;
	/** 获取当前TurningInPlaceType */
	FORCEINLINE ETurningInPlace GetTurningInPlaceType() const { return TurningInPlaceType; }

protected:
	/** 拾取操作 */
	void OnPickupInput();

	/** 下蹲操作 */
	void OnCrouchStartInput();
	void OnCrouchEndInput();

	/** 瞄准操作 */
	void OnAimStartInput();
	void OnAimEndInput();

	/** 开火操作 */
	void OnFireStartInput();
	void OnFireEndInput();

private:
	UPROPERTY(ReplicatedUsing = OnRep_OverlapWeapon)
	APPWeapon* OverlapWeapon;
	
	FTimerHandle ShowPlayerNameHandle;

	float AO_Yaw;
	float AO_Pitch;
	FRotator StartAimRotation;
	ETurningInPlace TurningInPlaceType = ETurningInPlace::ETIP_TurnNone;
	float InterpAO_Yaw;
};
