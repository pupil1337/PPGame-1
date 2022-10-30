// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	ETIP_TurnNone UMETA("Turn None"),
	ETIP_TurnLeft UMETA("Turn Left"),
	ETIP_TurnRight UMETA("Turn Right"),

	ETIP_MAX UMETA("DefaultMAX")
};