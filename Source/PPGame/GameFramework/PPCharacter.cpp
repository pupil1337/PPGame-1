﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "PPGame/Component/PPCombatComponent.h"
#include "PPGame/UMG/WidgetComponent/PPShowPlayerName.h"
#include "PPGame/Weapon/PPWeapon.h"
#include "kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"


APPCharacter::APPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->SetCrouchedHalfHeight(60.0f);
	
	// PlayerNameComponent
	PlayerNameComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("ShowPlayerName"));
	PlayerNameComp->SetupAttachment(RootComponent);
	PlayerNameComp->SetVisibility(false);

	// CombatComponent
	CombatComp = CreateDefaultSubobject<UPPCombatComponent>(TEXT("CombatComponent"));
	CombatComp->SetIsReplicated(true);
}

void APPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APPCharacter, OverlapWeapon, COND_OwnerOnly);
}

void APPCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComp)
	{
		CombatComp->PPCharacter = this;
	}
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
						if (IA_Pickup)
						{
							EnhancedInputComponent->BindAction(IA_Pickup, ETriggerEvent::Triggered, this, &ThisClass::OnPickupInput);
						}
						if (IA_CrouchStart)
						{
							EnhancedInputComponent->BindAction(IA_CrouchStart, ETriggerEvent::Triggered, this, &ThisClass::OnCrouchStartInput);
						}
						if (IA_CrouchEnd)
						{
							EnhancedInputComponent->BindAction(IA_CrouchEnd, ETriggerEvent::Triggered, this, &ThisClass::OnCrouchEndInput);
						}
						if (IA_AimStart)
						{
							EnhancedInputComponent->BindAction(IA_AimStart, ETriggerEvent::Triggered, this, &ThisClass::OnAimStartInput);
						}
						if (IA_AimEnd)
						{
							EnhancedInputComponent->BindAction(IA_AimEnd, ETriggerEvent::Triggered, this, &ThisClass::OnAimEndInput);
						}
						if (IA_FireStart)
						{
							EnhancedInputComponent->BindAction(IA_FireStart, ETriggerEvent::Triggered, this, &ThisClass::OnFireStartInput);
						}
						if (IA_FireEnd)
						{
							EnhancedInputComponent->BindAction(IA_FireEnd, ETriggerEvent::Triggered, this, &ThisClass::OnFireEndInput);
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

void APPCharacter::AimOffset(float DeltaTime)
{
	if (CombatComp && CombatComp->EquippedWeapon != nullptr)
	{
		FVector Velocity = GetVelocity();
		Velocity.Z = 0;
		float Speed = Velocity.Size();
		bool bIsInAir = GetMovementComponent()->IsFalling();
		FRotator BaseAimRotation = GetBaseAimRotation();

		// 1.AO_Yaw
		if (Speed == 0.0f && !bIsInAir) // 站立不动
		{
			FRotator CurrAimRotation = FRotator(0.0f, BaseAimRotation.Yaw, 0.0f);
			FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrAimRotation, StartAimRotation);
			AO_Yaw = DeltaAimRotation.Yaw;

			if (AO_Yaw < -70.0f)
			{
				TurningInPlaceType = ETurningInPlace::ETIP_TurnLeft;
			}
			else if (AO_Yaw > 70.0f)
			{
				TurningInPlaceType = ETurningInPlace::ETIP_TurnRight;
			}
			
			if (TurningInPlaceType != ETurningInPlace::ETIP_TurnNone)
			{
				AO_Yaw = InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.0f, DeltaTime, 4.0f);
				if (FMath::Abs(AO_Yaw) < 10.0f)
				{
					StartAimRotation = FRotator(0.0f, BaseAimRotation.Yaw, 0.0f);
					AO_Yaw = 0.0f;
					TurningInPlaceType = ETurningInPlace::ETIP_TurnNone;
				}
			}
			else
			{
				InterpAO_Yaw = AO_Yaw;
			}
		}
		else // 移动中
		{
			StartAimRotation = FRotator(0.0f, BaseAimRotation.Yaw, 0.0f);
			AO_Yaw = 0.0f;
			TurningInPlaceType = ETurningInPlace::ETIP_TurnNone;
		}

		// 2.AO_Pitch
		AO_Pitch = BaseAimRotation.Pitch;
		if (!IsLocallyControlled() && AO_Pitch > 90.0f)
		{
			FVector2D InRange = {270.0f, 360.0f};
			FVector2D OutRange = {-90.0f, 0.0f};
			AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
		}
	}
}

void APPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AimOffset(DeltaTime);
}

void APPCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void APPCharacter::Destroyed()
{
	GetWorldTimerManager().ClearTimer(ShowPlayerNameHandle);

	Super::Destroyed();
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

void APPCharacter::PlayFireMontage(bool bAiming)
{
	if (FireWeaponMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(FireWeaponMontage);
			FName FireSectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
			AnimInstance->Montage_JumpToSection(FireSectionName);
		}
	}
}

/** Input ================================================================================= */

void APPCharacter::OnPickupInput()
{
	if (CombatComp)
	{
		CombatComp->EquipWeapon(OverlapWeapon);
	}
}

void APPCharacter::OnCrouchStartInput()
{
	Crouch();
}

void APPCharacter::OnCrouchEndInput()
{
	UnCrouch();
}

void APPCharacter::OnAimStartInput()
{
	if (CombatComp)
	{
		CombatComp->Aim(true);
	}
}

void APPCharacter::OnAimEndInput()
{
	if (CombatComp)
	{
		CombatComp->Aim(false);
	}
}

void APPCharacter::OnFireStartInput()
{
	if (CombatComp)
	{
		CombatComp->Fire(true);
	}
}

void APPCharacter::OnFireEndInput()
{
	if (CombatComp)
	{
		CombatComp->Fire(false);
	}
}

/** Getter ================================================================================= */

bool APPCharacter::GetIsEquipWeapon() const
{
	return (CombatComp && CombatComp->EquippedWeapon);
}

bool APPCharacter::GetIsAiming() const
{
	return (CombatComp && CombatComp->bAiming);
}

APPWeapon* APPCharacter::GetEquippedWeapon() const
{
	if (GetIsEquipWeapon())
	{
		return CombatComp->EquippedWeapon;
	}
	return nullptr;
}

