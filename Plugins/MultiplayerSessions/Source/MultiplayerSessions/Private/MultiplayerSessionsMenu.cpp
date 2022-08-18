// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsMenu.h"

#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"

bool UMultiplayerSessionsMenu::Initialize()
{
	if (Super::Initialize())
	{
		BindButton();
		SetUpMenu();
		return true;
	}

	return false;
}

void UMultiplayerSessionsMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	TearDownMenu();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UMultiplayerSessionsMenu::BindButton()
{
	if (Button_Host)
	{
		Button_Host->OnClicked.AddUniqueDynamic(this, &ThisClass::OnHostButtonClicked);
	}

	if (Button_Join)
	{
		Button_Join->OnClicked.AddUniqueDynamic(this, &ThisClass::OnJoinButtonClicked);
	}
}

void UMultiplayerSessionsMenu::SetUpMenu()
{
	// 设置Widget
	AddToViewport();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	bIsFocusable = true;

	if (const UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			FInputModeUIOnly InputModeUIOnly;
			InputModeUIOnly.SetWidgetToFocus(TakeWidget());
			InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			PlayerController->SetInputMode(InputModeUIOnly);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	// 初始化变量
	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerSessionsOnCreateSessionComplete.AddUObject(this, &ThisClass::OnCreateSessionComplete);
		MultiplayerSessionsSubsystem->MultiplayerSessionsOnStartSessionComplete.AddUObject(this, &ThisClass::OnStartSessionComplete);
		MultiplayerSessionsSubsystem->MultiplayerSessionsOnDestroySessionComplete.AddUObject(this, &ThisClass::OnDestroySessionComplete);
		MultiplayerSessionsSubsystem->MultiplayerSessionsOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessionsComplete);
		MultiplayerSessionsSubsystem->MultiplayerSessionsOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSessionComplete);
	}
}

void UMultiplayerSessionsMenu::TearDownMenu() const
{
	if (const UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			const FInputModeGameOnly InputModeGameOnly;

			PlayerController->SetInputMode(InputModeGameOnly);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

void UMultiplayerSessionsMenu::OnHostButtonClicked()
{
	Button_Host->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->CreateSession();
	}
}

void UMultiplayerSessionsMenu::OnJoinButtonClicked()
{
	Button_Join->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(10000);
	}
}

void UMultiplayerSessionsMenu::OnCreateSessionComplete(bool bWasSuccessful)
{
	ScreenLog(FString::Printf(TEXT("Create Success: %hs"), bWasSuccessful ? "true" : "false"))

	// todo: 创建Session之后Session处于Pending状态,StartSession()完成后才是InProgress状态,不应当此时跳转地图
	if (bWasSuccessful)
	{
		if (UWorld* World = GetWorld())
		{
			World->ServerTravel("/MultiplayerSessions/MultiplayerSessionsMap_Lobby?listen");
		}
	}
	else
	{
		Button_Host->SetIsEnabled(true);
	}
}

void UMultiplayerSessionsMenu::OnStartSessionComplete(bool bWasSuccessful)
{
}

void UMultiplayerSessionsMenu::OnDestroySessionComplete(bool bWasSuccessful)
{
}

void UMultiplayerSessionsMenu::OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& Results, bool bWasSuccessful)
{
	ScreenLog(FString::Printf(TEXT("FindSessions Num: %d"), Results.Num()))
	
	if (bWasSuccessful && Results.Num() > 0)
	{
		if (MultiplayerSessionsSubsystem)
		{
			for (const FOnlineSessionSearchResult& Result: Results)
			{
				// todo: 手动选择想加入的Session
				FString MatchType;
				Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);
				if (MatchType == "PupilTest")
				{
					ScreenLog(FString(TEXT("Join the First PupilTest")))
					MultiplayerSessionsSubsystem->JoinSession(Result);
					return;
				}
			}
		}
	}
	else
	{
		Button_Join->SetIsEnabled(true);
	}
}

void UMultiplayerSessionsMenu::OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result, FString Address)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}
	}
	else
	{
		ScreenLog(FString(TEXT("JoinSession Failed! -- %d"), static_cast<uint32>(Result)))
		Button_Join->SetIsEnabled(true);
	}
}
