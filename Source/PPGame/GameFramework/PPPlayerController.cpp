// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PPCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "PPGame/Camera/PPPlayerCameraManager.h"

void APPPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	SetUpCamera(GetPawn());
}

void APPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (BaseIMC)
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(BaseIMC, 9);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (IA_MoveForward)
		{
			EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &ThisClass::OnMoveForward);
		}
		if (IA_MoveRight)
		{
			EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &ThisClass::OnMoveRight);
		}
		if (IA_TurnRight)
		{
			EnhancedInputComponent->BindAction(IA_TurnRight, ETriggerEvent::Triggered, this, &ThisClass::OnTurnRight);
		}
		if (IA_TurnUp)
		{
			EnhancedInputComponent->BindAction(IA_TurnUp, ETriggerEvent::Triggered, this, &ThisClass::OnTurnUp);
		}
		if (IA_ShowPlayerName)
		{
			EnhancedInputComponent->BindAction(IA_ShowPlayerName, ETriggerEvent::Triggered, this, &ThisClass::OnShowPlayerName);
		}
	}
}

void APPPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetUpCamera(InPawn);
}

void APPPlayerController::OnMoveForward(const FInputActionValue& Value)
{
	if (APPCharacter* PPCharacter = Cast<APPCharacter>(GetPawn()))
	{
		PPCharacter->AddMovementInput(FRotationMatrix({0.0f, GetControlRotation().Yaw, 0.0f}).GetUnitAxis(EAxis::X), Value.GetMagnitude());
	}
}

void APPPlayerController::OnMoveRight(const FInputActionValue& Value)
{
	if (APPCharacter* PPCharacter = Cast<APPCharacter>(GetPawn()))
	{
		PPCharacter->AddMovementInput(FRotationMatrix({0.0f, GetControlRotation().Yaw, 0.0f}).GetUnitAxis(EAxis::Y), Value.GetMagnitude());
	}
}

void APPPlayerController::OnTurnRight(const FInputActionValue& Value)
{
	AddYawInput(Value.GetMagnitude());
}

void APPPlayerController::OnTurnUp(const FInputActionValue& Value)
{
	AddPitchInput(Value.GetMagnitude());

	// 限制输入角度 [0, 60] [270, 360]
	const float NewRotationPitch = RotationInput.Pitch + GetControlRotation().Pitch;
	if (NewRotationPitch > 60.0f && NewRotationPitch < 270.0f)
	{
		RotationInput.Pitch = 0.0f;
	}
}

void APPPlayerController::OnShowPlayerName(const FInputActionValue& Value)
{
	if (AGameStateBase* GameState = UGameplayStatics::GetGameState(this))
	{
		if (GameState->PlayerArray.Num() > 0)
		{
			for (auto it: GameState->PlayerArray)
			{
				if (it.Get() && it.Get()->GetPawn())
				{
					if (APPCharacter* ppCharacter = Cast<APPCharacter>(it.Get()->GetPawn()))
					{
						ppCharacter->SetOverheadPlayerName();
					}
				}
			}
		}
	}
}

void APPPlayerController::SetUpCamera(APawn* InPawn)
{
	if (InPawn && PlayerCameraManager)
	{
		if (APPPlayerCameraManager* PPPlayerCameraManager = Cast<APPPlayerCameraManager>(PlayerCameraManager))
		{
			PPPlayerCameraManager->OnPossess(Cast<APPCharacter>(InPawn));
		}
	}
}

