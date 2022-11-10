// Fill out your copyright notice in the Description page of Project Settings.


#include "PPProjectileWeapon.h"
#include "PPProjectile.h"
#include "Engine/SkeletalMeshSocket.h"

APPProjectileWeapon::APPProjectileWeapon()
{
	
}

void APPProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	if (ProjectileClass && GetWorld() && GetMesh())
	{
		if (const USkeletalMeshSocket* MuzzleFlashSocket = GetMesh()->GetSocketByName(FName("MuzzleFlash")))
		{
			FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetMesh());
			FVector ToTarget = HitTarget - SocketTransform.GetLocation();
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = GetOwner();
			SpawnParameters.Instigator = Cast<APawn>(GetOwner());
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APPProjectile>(
				ProjectileClass,
				SocketTransform.GetLocation(),
				ToTarget.Rotation(),
				SpawnParameters
			);
		}
	}
}

