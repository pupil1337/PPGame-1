// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacterCameraBehavior.h"

void UPPCharacterCameraBehavior::SetRotationMode(EPPRotationMode RotationMode)
{
	bVelocityDirection = RotationMode == EPPRotationMode::VelocityDirection;
	bLookingDirection = RotationMode == EPPRotationMode::LookingDirection;
	bAiming = RotationMode == EPPRotationMode::Aiming;
}
