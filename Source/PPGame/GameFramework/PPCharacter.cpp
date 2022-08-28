// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "PPGame/UMG/WidgetComponent/PPShowPlayerName.h"
#include "PPGame/Weapon/PPWeapon.h"


APPCharacter::APPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	PlayerNameComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("ShowPlayerName"));
	PlayerNameComp->SetupAttachment(RootComponent);
	PlayerNameComp->SetVisibility(false);
}

void APPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APPCharacter, OverlapWeapon, COND_OwnerOnly)
}

void APPCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APPCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void APPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				if (CharacterIMC)
				{
					EnhancedInputLocalPlayerSubsystem->AddMappingContext(CharacterIMC, 0);

					if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
					{
						if (IA_Jump)
						{
							EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ThisClass::Jump);
						}
					}
				}
			}
		}
	}
}

void APPCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APPCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void APPCharacter::Destroyed()
{
	Super::Destroyed();

	GetWorldTimerManager().ClearTimer(ShowPlayerNameHandle);
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

void APPCharacter::SetOverlapWeapon(APPWeapon* NewOverlapWeapon)
{
	APPWeapon* OldOverlapWeapon = OverlapWeapon;
	OverlapWeapon = NewOverlapWeapon;
	if (IsLocallyControlled())
	{
		OnRep_OverlapWeapon(OldOverlapWeapon);
	}
}

void APPCharacter::OnRep_OverlapWeapon(APPWeapon* OldOverlapWeapon)
{
	if (OldOverlapWeapon)
	{
		OldOverlapWeapon->SetPickupTipVisibility(false);
	}
	if (OverlapWeapon)
	{
		OverlapWeapon->SetPickupTipVisibility(true);
	}
}

