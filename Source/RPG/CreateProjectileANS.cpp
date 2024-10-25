// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateProjectileANS.h"
#include "MonsterProjectile.h"

void UCreateProjectileANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (auto* World = MeshComp->GetWorld()) {
		FVector SpawnLocation = MeshComp->GetComponentLocation() + MeshComp->GetForwardVector() * 100.f;
		FRotator SpawnRotation = MeshComp->GetComponentRotation();

		if (auto* Projectile = World->SpawnActor<AMonsterProjectile>(MonsterProjecileClass, SpawnLocation, SpawnRotation)) {
			/*if (Projectile) {
				Projectile->StartHomingToPlayer();
			}*/
		}
	}
}
