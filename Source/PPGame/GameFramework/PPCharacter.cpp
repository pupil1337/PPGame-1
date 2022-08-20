// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

const FName NAME_FP_Camera(TEXT("FP_Camera"));

APPCharacter::APPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void APPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APPCharacter::GetCameraParameters(float& TPFOVOut, float& FPFOVOut, bool& bRightShoulderOut) const
{
	TPFOVOut = ThirdPersonFOV;
	FPFOVOut = FirstPersonFOV;
	bRightShoulderOut = bRightShoulder;
}

FTransform APPCharacter::GetThirdPersonPivotTarget() const
{
	return GetActorTransform();
}

FVector APPCharacter::GetFirstPersonCameraTarget() const
{
	return GetMesh()->GetSocketLocation(NAME_FP_Camera);
}

ECollisionChannel APPCharacter::GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius)
{
	TraceOrigin = GetActorLocation();
	TraceRadius = 10.0f;
	return ECC_Visibility;
}

