// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PPCharacter.generated.h"

class UWidgetComponent;

UCLASS()
class PPGAME_API APPCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> PlayerNameComp;
	
public:
	APPCharacter();
	/** 显示角色头顶Steam昵称 */
	virtual void SetOverheadPlayerName();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnRep_PlayerState() override;

	virtual void Destroyed() override;

private:
	FTimerHandle ShowPlayerNameHandle;
};
