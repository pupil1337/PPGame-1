// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPAniminstanceEnumLibrary.generated.h"

/**
 * Character转向方向
 */
UENUM(BlueprintType)
enum class EPPRotationMode: uint8
{
	VelocityDirection, // 速度方向
	LookingDirection,  // 观察方向
	Aiming             // 瞄准方向
};

/**
 * Character 移动状态
 */
UENUM()
enum class EPPMovementState: uint8
{
	Grounded, // 地面
	InAir,    // 空中
	Mantling, // 攀爬
	RagDoll   // 布娃娃
};

/**
 * Character 姿势
 */
UENUM()
enum class EPPStance: uint8
{
	Standing,
	Crouching
};
