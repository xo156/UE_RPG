// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterProjectileNotify.h"
#include "MonsterProjectile.h"

void UMonsterProjectileNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (AMonsterProjectileClass) {
			FVector SpawnLocation = MeshComp->GetSocketLocation(FName("MonsterProjectile")) + FVector(100.f, 100.f, 100.f);
			auto* MonsterProjectile = MeshComp->GetWorld()->SpawnActor<AMonsterProjectile>(AMonsterProjectileClass, SpawnLocation, FRotator::ZeroRotator);
			if (MonsterProjectile) {
				AActor* OwnerActor = MeshComp->GetOwner();
				MonsterProjectile->SetOwner(OwnerActor);
				MonsterProjectile->SetInstigator(Cast<APawn>(OwnerActor));
				MonsterProjectile->DamageAmount = 10.f;
			}
		}
	}
}
