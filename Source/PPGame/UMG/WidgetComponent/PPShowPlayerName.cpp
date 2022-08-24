// Fill out your copyright notice in the Description page of Project Settings.


#include "PPShowPlayerName.h"

#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UPPShowPlayerName::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UPPShowPlayerName::SetPlayerName(const FString& InString)
{
	TextBlock_PlayerName->SetText(FText::FromString(InString));
}
