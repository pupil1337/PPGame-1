﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "PPPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class APPCharacter;

/**
 * 
 */
UCLASS()
class PPGAME_API APPPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = EnhancedInput)
	TObjectPtr<UInputMappingContext> BaseIMC;

	UPROPERTY(EditDefaultsOnly, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_MoveForward;
	UPROPERTY(EditDefaultsOnly, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_MoveRight;
	UPROPERTY(EditDefaultsOnly, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_TurnRight;
	UPROPERTY(EditDefaultsOnly, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_TurnUp;
	UPROPERTY(EditDefaultsOnly, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_ShowPlayerName;

public:
	virtual void OnRep_Pawn() override;
	
protected:
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	void OnMoveForward(const FInputActionValue& Value);
	void OnMoveRight(const FInputActionValue& Value);
	void OnTurnRight(const FInputActionValue& Value);
	void OnTurnUp(const FInputActionValue& Value);
	void OnShowPlayerName(const FInputActionValue& Value);

	void SetUpCamera(APawn* InPawn);
};
