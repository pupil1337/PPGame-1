// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PPCharacter.generated.h"

UCLASS()
class PPGAME_API APPCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "PPCharacter|Camera System")
	float ThirdPersonFOV = 90.0f;
	UPROPERTY(EditDefaultsOnly, Category = "PPCharacter|Camera System")
	float FirstPersonFOV = 90.0f;
	UPROPERTY(EditDefaultsOnly, Category = "PPCharacter|Camera System")
	bool bRightShoulder = true;
	
public:
	APPCharacter();
	void GetCameraParameters(float& TPFOVOut, float& FPFOVOut, bool& bRightShoulderOut) const;
	FTransform GetThirdPersonPivotTarget() const;
	FVector GetFirstPersonCameraTarget() const;
	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
