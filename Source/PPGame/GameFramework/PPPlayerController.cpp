// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PPCharacter.h"
#include "PPGame/Camera/PPPlayerCameraManager.h"

void APPPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	SetUpCamera(GetPawn());
}

void APPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	check(Cast<UEnhancedInputComponent>(InputComponent))
	check(BaseIMC.Get())
	check(IA_MoveForward.Get())
	check(IA_MoveRight.Get())
	check(IA_Jump.Get())
	check(IA_TurnRight.Get())
	check(IA_TurnUp.Get())

	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (BaseIMC)
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(BaseIMC, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (IA_MoveForward)
		{
			EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &ThisClass::MoveForward);
		}
		if (IA_MoveRight)
		{
			EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &ThisClass::MoveRight);
		}
		if (IA_Jump)
		{
			EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ThisClass::MoveJump);
		}
		if (IA_TurnRight)
		{
			EnhancedInputComponent->BindAction(IA_TurnRight, ETriggerEvent::Triggered, this, &ThisClass::TurnRight);
		}
		if (IA_TurnUp)
		{
			EnhancedInputComponent->BindAction(IA_TurnUp, ETriggerEvent::Triggered, this, &ThisClass::TurnUp);
		}
	}
}

void APPPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetUpCamera(InPawn);
}

void APPPlayerController::MoveForward(const FInputActionValue& Value)
{
	if (APPCharacter* PPCharacter = Cast<APPCharacter>(GetPawn()))
	{
		PPCharacter->AddMovementInput(FRotationMatrix({0.0f, GetControlRotation().Yaw, 0.0f}).GetUnitAxis(EAxis::X), Value.GetMagnitude());
	}
}

void APPPlayerController::MoveRight(const FInputActionValue& Value)
{
	if (APPCharacter* PPCharacter = Cast<APPCharacter>(GetPawn()))
	{
		PPCharacter->AddMovementInput(FRotationMatrix({0.0f, GetControlRotation().Yaw, 0.0f}).GetUnitAxis(EAxis::Y), Value.GetMagnitude());
	}
}

void APPPlayerController::MoveJump(const FInputActionValue& Value)
{
	if (APPCharacter* PPCharacter = Cast<APPCharacter>(GetPawn()))
	{
		PPCharacter->Jump();
	}
}

void APPPlayerController::TurnRight(const FInputActionValue& Value)
{
	AddYawInput(Value.GetMagnitude());
}

void APPPlayerController::TurnUp(const FInputActionValue& Value)
{
	AddPitchInput(Value.GetMagnitude());
}

void APPPlayerController::SetUpCamera(APawn* InPawn)
{
	check(Cast<APPCharacter>(InPawn))
	check(Cast<APPPlayerCameraManager>(PlayerCameraManager))
	
	if (APPPlayerCameraManager* PPPlayerCameraManager = Cast<APPPlayerCameraManager>(PlayerCameraManager))
	{
		PPPlayerCameraManager->OnPossess(Cast<APPCharacter>(InPawn));
	}
}

