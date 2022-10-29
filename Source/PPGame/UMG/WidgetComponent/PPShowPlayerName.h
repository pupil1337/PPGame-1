// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPShowPlayerName.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PPGAME_API UPPShowPlayerName : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_PlayerName;

public:
	void SetPlayerName(const FString& InString);
	
protected:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
};
