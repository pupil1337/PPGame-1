// Fill out your copyright notice in the Description page of Project Settings.


#include "PPAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "PPGame/GameFramework/PPCharacter.h"

void UPPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PPCharacter = Cast<APPCharacter>(TryGetPawnOwner());
}

void UPPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (PPCharacter == nullptr)
	{
		PPCharacter = Cast<APPCharacter>(TryGetPawnOwner());
	}
	if (PPCharacter == nullptr)
	{
		#if !WITH_EDITOR
			UE_LOG(LogTemp, Warning, TEXT("NativeUpdateAnimation:: Cast<APPCharacter>(TryGetPawnOwner()) == nullptr"))
		#endif

		return;
	}

	// 1.Speed
	FVector Velocity = PPCharacter->GetVelocity();
	Velocity.Z = 0;
	Speed = Velocity.Size();

	// 2.bIsInAir
	bIsInAir = PPCharacter->GetCharacterMovement()->IsFalling();

	// 3.bIsAccelerating
	bIsAccelerating = PPCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f ? true : false;

	// 4.bEquipWeapon
	bEquipWeapon = PPCharacter->GetIsEquipWeapon();

	// 5.bIsCrouched
	bIsCrouched = PPCharacter->bIsCrouched;

	// 6.bAiming
	bAiming = PPCharacter->GetIsAiming();
}
