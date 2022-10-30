// Fill out your copyright notice in the Description page of Project Settings.


#include "PPAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PPGame/GameFramework/PPCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "PPGame/Weapon/PPWeapon.h"

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

	// 7.Yaw
	FRotator AimRotation = PPCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(PPCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaSeconds, 6.0f);
	Yaw = DeltaRotation.Yaw;
	
	// 8.Lean
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = PPCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaSeconds;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 6.0f);
	Lean = FMath::Clamp(Interp, -30.0f, 30.0f);

	// 9.AO_Yaw & AO_Pitch
	AO_Yaw = PPCharacter->GetAO_Yaw();
	AO_Pitch = PPCharacter->GetAO_Pitch();

	// 10.LeftHandTransform
	if (APPWeapon* PPWeapon = PPCharacter->GetEquippedWeapon())
	{
		if (USkeletalMeshComponent* WeaponMesh = PPWeapon->GetMesh())
		{
			LeftHandTransform = WeaponMesh->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
			FVector OutPosition;
			FRotator OutRotation;
			PPCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
			LeftHandTransform.SetLocation(OutPosition);
			LeftHandTransform.SetRotation(FQuat(OutRotation));
		}
	}

	// 11.TurningInPlaceType
	TurningInPlaceType = PPCharacter->GetTurningInPlaceType();
}
