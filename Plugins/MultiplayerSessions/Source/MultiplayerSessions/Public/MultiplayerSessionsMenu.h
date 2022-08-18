// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionsMenu.generated.h"

class UButton;
class UMultiplayerSessionsSubsystem;

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsMenu : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Host;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Join;

public:
	virtual bool Initialize() override;

protected:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

private:
	void BindButton();
	void SetUpMenu();
	void TearDownMenu() const;

	UFUNCTION()
	void OnHostButtonClicked();
	UFUNCTION()
	void OnJoinButtonClicked();

	void OnCreateSessionComplete(bool bWasSuccessful);
	void OnStartSessionComplete(bool bWasSuccessful);
	void OnDestroySessionComplete(bool bWasSuccessful);
	void OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& Results, bool bWasSuccessful);
	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result, FString Address);

	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
};
