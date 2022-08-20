// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PPGame/Library/PPAniminstanceEnumLibrary.h"
#include "PPCharacterCameraBehavior.generated.h"

/**
 * 
 */
UCLASS()
class PPGAME_API UPPCharacterCameraBehavior : public UAnimInstance
{
	GENERATED_BODY()

public:
	void SetRotationMode(EPPRotationMode RotationMode);

	// 移动状态
	
	
protected:
	// 转向方式
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraInstance")
	bool bLookingDirection = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraInstance")
	bool bVelocityDirection = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraInstance")
	bool bAiming = false;
};
