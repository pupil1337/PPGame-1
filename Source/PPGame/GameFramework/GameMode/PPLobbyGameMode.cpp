// Copyright Epic Games, Inc. All Rights Reserved.


#include "PPLobbyGameMode.h"

#include "MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameStateBase.h"

APPLobbyGameMode::APPLobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void APPLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	MultiplayerSessionsSubsystem = UGameInstance::GetSubsystem<UMultiplayerSessionsSubsystem>(GetGameInstance());
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerSessionsOnStartSessionComplete.AddUObject(this, &ThisClass::OnStartSession);
	}
}

void APPLobbyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
}

void APPLobbyGameMode::OnStartSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (UWorld* World = GetWorld())
		{
			World->ServerTravel("/Game/Maps/PP/PPMap?listen");
		}
	}
}
