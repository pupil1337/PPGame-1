// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	ETIP_TurnNone UMETA(DisplayName = "Turn None"),
	ETIP_TurnLeft UMETA(DisplayName = "Turn Left"),
	ETIP_TurnRight UMETA(DisplayName = "Turn Right"),

	ETIP_MAX UMETA(DisplayName = "DefaultMAX")
};