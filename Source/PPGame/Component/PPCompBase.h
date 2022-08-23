// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PPCompBase.generated.h"

UENUM()
enum class EPPCompSpawnCondition: uint8
{
	/** 所有端都生成 */
	EPPCSC_Multi = 0,
	/** 仅在服务器生成 */
	EPPCSC_OnlyOnServer = 1,
	/** 仅在所有客户端生成 */
	EPPCSC_OnlyOnClients = 2,
	/** 仅在自主代理客户端生成 */
	EPPCSC_OnlyOnAutonomous = 3,
	/** 仅在模拟代理客户端生成 */
	EPPCSC_OnlyOnSimulated = 4,
};

UCLASS( Abstract)
class PPGAME_API UPPCompBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly)
	EPPCompSpawnCondition PPCompSpawnCondition = EPPCompSpawnCondition::EPPCSC_Multi;
};
