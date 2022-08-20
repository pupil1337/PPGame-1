// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "PPPlayerCameraManager.generated.h"

class APPCharacter;

/**
 * 
 */
UCLASS()
class PPGAME_API APPPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> CameraBehavior = nullptr;

public:
	APPPlayerCameraManager();

	void OnPossess(APPCharacter* NewCharacter);

protected:
	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;

private:
	float GetCameraBehaviorParam(FName CurveName) const;
	FVector CalculateAxisIndependentLag(FVector CurrentLocation, FVector TargetLocation, FRotator CameraRotation, FVector LagSpeeds, float DeltaTime);
	bool CustomCameraBehavior(float DeltaTime, FVector& Location, FRotator& Rotation, float& FOV);
	
	UPROPERTY()
	TObjectPtr<APPCharacter> ControlledCharacter = nullptr;
	
	UPROPERTY()
	FTransform SmoothedPivotTarget;
	UPROPERTY()
	FVector PivotLocation;
	UPROPERTY()
	FVector TargetCameraLocation;
	UPROPERTY()
	FRotator TargetCameraRotation;
};
