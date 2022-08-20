// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "PPPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

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
	TObjectPtr<UInputAction> IA_Jump;
	UPROPERTY(EditDefaultsOnly, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_TurnRight;
	UPROPERTY(EditDefaultsOnly, Category = EnhancedInput)
	TObjectPtr<UInputAction> IA_TurnUp;
	
protected:
	virtual void SetupInputComponent() override;

private:
	virtual void MoveForward(const FInputActionValue& Value);
	virtual void MoveRight(const FInputActionValue& Value);
	virtual void MoveJump(const FInputActionValue& Value);
	virtual void TurnRight(const FInputActionValue& Value);
	virtual void TurnUp(const FInputActionValue& Value);
};
