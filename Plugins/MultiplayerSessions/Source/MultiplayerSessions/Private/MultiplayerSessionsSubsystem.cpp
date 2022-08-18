// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
	:
	// 创建Session代理
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	StartSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete))
{
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
		
		ScreenLog(FString::Printf(TEXT("OnlineSubsystem is: %s"), *OnlineSubsystem->GetSubsystemName().ToString()))
	}
}

/* --public-- ========================================================================*/

void UMultiplayerSessionsSubsystem::CreateSession()
{
	if (OnlineSessionInterface.IsValid())
	{
		// 销毁已经创建的Session
		if (OnlineSessionInterface->GetNamedSession(NAME_GameSession))
		{
			bCreateSessionAfterDestroyed = true;
			DestroySession();
			return;
		}

		// 添加到代理列表
		CreateSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

		// todo: 从UI设置Settings
		// SessionSettings和创建Session
		OnlineSessionSettings = MakeShareable(new FOnlineSessionSettings);
		OnlineSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
		OnlineSessionSettings->NumPublicConnections = 4;
		OnlineSessionSettings->bAllowJoinInProgress = true;
		OnlineSessionSettings->bAllowJoinViaPresence = true;
		OnlineSessionSettings->bUsesPresence = true;
		OnlineSessionSettings->bShouldAdvertise = true;
		OnlineSessionSettings->bUseLobbiesIfAvailable = true;
		OnlineSessionSettings->BuildUniqueId = true;
		OnlineSessionSettings->Set(FName("MatchType"), FString("PupilTest"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		if (!OnlineSessionInterface->CreateSession(*GetWorld()->GetFirstLocalPlayerFromController()->GetPreferredUniqueNetId(), NAME_GameSession, *OnlineSessionSettings))
		{
			OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

			MultiplayerSessionsOnCreateSessionComplete.Broadcast(false);
		}
	}
}

void UMultiplayerSessionsSubsystem::StartSession()
{
	if (OnlineSessionInterface.IsValid())
	{
		StartSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);

		if (!OnlineSessionInterface->StartSession(NAME_GameSession))
		{
			OnlineSessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);

			MultiplayerSessionsOnCreateSessionComplete.Broadcast(false);
		}
	}
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
	if (OnlineSessionInterface.IsValid())
	{
		DestroySessionCompleteDelegateHandle = OnlineSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

		if (!OnlineSessionInterface->DestroySession(NAME_GameSession))
		{
			OnlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);

			MultiplayerSessionsOnDestroySessionComplete.Broadcast(false);
		}
	}
}

void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
	if (OnlineSessionInterface.IsValid())
	{
		FindSessionsCompleteDelegateHandle = OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

		OnlineSessionSearch = MakeShareable(new FOnlineSessionSearch());
		OnlineSessionSearch->MaxSearchResults = MaxSearchResults;
		OnlineSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
		OnlineSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		if (!OnlineSessionInterface->FindSessions(*GetWorld()->GetFirstLocalPlayerFromController()->GetPreferredUniqueNetId(), OnlineSessionSearch.ToSharedRef()))
		{
			OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

			MultiplayerSessionsOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		}
	}
}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& Result)
{
	if (OnlineSessionInterface.IsValid())
	{
		JoinSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

		if (!OnlineSessionInterface->JoinSession(*GetWorld()->GetFirstLocalPlayerFromController()->GetPreferredUniqueNetId(), NAME_GameSession, Result))
		{
			OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

			MultiplayerSessionsOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError, FString(""));
		}
	}
	else
	{
		MultiplayerSessionsOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError, FString(""));
	}
}

/* --protected-- ========================================================================*/

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (OnlineSessionInterface.IsValid())
	{
		OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
	
	MultiplayerSessionsOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (OnlineSessionInterface.IsValid())
	{
		OnlineSessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
	}
	
	MultiplayerSessionsOnStartSessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (OnlineSessionInterface.IsValid())
	{
		OnlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}

	if (bWasSuccessful && bCreateSessionAfterDestroyed)
	{
		bCreateSessionAfterDestroyed = false;
		CreateSession();
	}
	
	MultiplayerSessionsOnDestroySessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (OnlineSessionInterface.IsValid())
	{
		OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}
	
	MultiplayerSessionsOnFindSessionsComplete.Broadcast(OnlineSessionSearch->SearchResults, bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type bWasSuccessful)
{
	if (OnlineSessionInterface.IsValid())
	{
		OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	FString Address;
	OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address);
	MultiplayerSessionsOnJoinSessionComplete.Broadcast(bWasSuccessful, Address);
}

/* --debug-- ========================================================================*/

void UMultiplayerSessionsSubsystem::DebugMultiplayerSession(int Type)
{
	if (OnlineSessionInterface.IsValid())
	{
		switch (Type)
		{
		case 1:
			ScreenLog(FString("Session State: ") + EOnlineSessionState::ToString(OnlineSessionInterface->GetSessionState(NAME_GameSession)))
			break;
		default:
			break;
		}
	}
}
