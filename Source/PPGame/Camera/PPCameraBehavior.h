// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PPGame/Library/PPAniminstanceEnumLibrary.h"
#include "PPCameraBehavior.generated.h"

/**
 * 
 */
UCLASS()
class PPGAME_API UPPCameraBehavior : public UAnimInstance
{
	GENERATED_BODY()

public:
	void SetRotationMode(EPPRotationMode RotationMode);

	// 移动状态
	
	
private:
	// 转向方式
	UPROPERTY(BlueprintReadOnly, Category = "CameraInstance", meta = (AllowPrivateAccess = "true"))
	bool bLookingDirection = false;

	UPROPERTY(BlueprintReadOnly, Category = "CameraInstance", meta = (AllowPrivateAccess = "true"))
	bool bVelocityDirection = false;

	UPROPERTY(BlueprintReadOnly, Category = "CameraInstance", meta = (AllowPrivateAccess = "true"))
	bool bAiming = false;
};
