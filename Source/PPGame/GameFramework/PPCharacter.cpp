// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacter.h"

APPCharacter::APPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

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

