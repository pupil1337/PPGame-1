// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PPCharacter.generated.h"

class UWidgetComponent;
class UInputMappingContext;
class UInputAction;
class APPWeapon;

UCLASS()
class PPGAME_API APPCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> PlayerNameComp;

	UPROPERTY(EditDefaultsOnly, Category = EnhancedInput)
	TObjectPtr<UInputMappingContext> CharacterIMC;
	UPROPERTY(EditDefaultsOnly, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_Jump;
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

public:
	/** 显示角色头顶Steam昵称 */
	virtual void SetOverheadPlayerName();
	/** 设置新的可拾取武器 */
	virtual void SetOverlapWeapon(APPWeapon* NewOverlapWeapon);
	UFUNCTION()
	virtual void OnRep_OverlapWeapon(APPWeapon* OldOverlapWeapon);

private:
	UPROPERTY(ReplicatedUsing = OnRep_OverlapWeapon)
	APPWeapon* OverlapWeapon;
	
	FTimerHandle ShowPlayerNameHandle;
};
