// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PPLobbyGameMode.generated.h"

class UMultiplayerSessionsSubsystem;

/**
 * 
 */
UCLASS()
class PPGAME_API APPLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APPLobbyGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;

private:
	virtual void OnStartSession(bool bWasSuccessful);
	
	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
};
