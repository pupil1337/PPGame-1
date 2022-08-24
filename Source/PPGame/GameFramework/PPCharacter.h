// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PPCharacter.generated.h"

class UWidgetComponent;

UCLASS()
class PPGAME_API APPCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> PlayerNameComp;

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
	/** 显示角色头顶Steam昵称 */
	virtual void SetOverheadPlayerName();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnRep_PlayerState() override;

	virtual void Destroyed() override;

private:
	FTimerHandle ShowPlayerNameHandle;
};
