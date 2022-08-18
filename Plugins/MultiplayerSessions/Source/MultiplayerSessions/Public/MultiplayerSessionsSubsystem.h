// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionsSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerSessionsOnCreateSessionComplete, bool bWasSuccessful)
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerSessionsOnStartSessionComplete, bool bWasSuccessful)
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerSessionsOnDestroySessionComplete, bool bWasSuccessful)
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerSessionsOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& Results, bool bWasSuccessful)
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerSessionsOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result, FString Address)

#define  ScreenLog( ParamFString ) \
	if (GEngine) \
	{ \
		GEngine->AddOnScreenDebugMessage( \
			-1, \
			15.0f, \
			FColor::Blue, \
			ParamFString \
		); \
	}

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMultiplayerSessionsSubsystem();

	// 外部调用事件
	void CreateSession();
	UFUNCTION(BlueprintCallable, Category = MultiplayerSessions)// todo: 删除UFUNCTION(BlueprintCallable)
	void StartSession();
	void DestroySession();
	void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& Result);

	// Session回调函数中所需调用代理
	FMultiplayerSessionsOnCreateSessionComplete MultiplayerSessionsOnCreateSessionComplete;
	FMultiplayerSessionsOnStartSessionComplete MultiplayerSessionsOnStartSessionComplete;
	FMultiplayerSessionsOnDestroySessionComplete MultiplayerSessionsOnDestroySessionComplete;
	FMultiplayerSessionsOnFindSessionsComplete MultiplayerSessionsOnFindSessionsComplete;
	FMultiplayerSessionsOnJoinSessionComplete MultiplayerSessionsOnJoinSessionComplete;
	
	UFUNCTION(BlueprintCallable, Category = MultiplayerSessions)
	void DebugMultiplayerSession(int Type);

protected:
	// Session事件的回调函数
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type bWasSuccessful);
	
private:
	IOnlineSessionPtr OnlineSessionInterface;
	TSharedPtr<FOnlineSessionSettings> OnlineSessionSettings;
	TSharedPtr<FOnlineSessionSearch> OnlineSessionSearch;
	bool bCreateSessionAfterDestroyed{ false };

	// 需要添加到Session代理列表的代理和其句柄
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
};
