// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacter.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "PPGame/UMG/WidgetComponent/PPShowPlayerName.h"


APPCharacter::APPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	PlayerNameComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("ShowPlayerName"));
	PlayerNameComp->SetupAttachment(RootComponent);
	PlayerNameComp->SetVisibility(false);
}

void APPCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void APPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APPCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APPCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void APPCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APPCharacter::SetOverheadPlayerName()
{
	if (UPPShowPlayerName* PlayerNameWidget = Cast<UPPShowPlayerName>(PlayerNameComp->GetWidget()))
	{
		if (APlayerState* tPlayerState = GetPlayerState())
		{
			PlayerNameComp->SetVisibility(true);
			PlayerNameWidget->SetPlayerName(tPlayerState->GetPlayerName());
			GetWorldTimerManager().ClearTimer(ShowPlayerNameHandle);
			GetWorldTimerManager().SetTimer(ShowPlayerNameHandle, FTimerDelegate::CreateLambda([this]()
			{
				PlayerNameComp->SetVisibility(false);
			}), 3.0f, false);
		}
	}
}

void APPCharacter::Destroyed()
{
	Super::Destroyed();

	GetWorldTimerManager().ClearTimer(ShowPlayerNameHandle);
}

