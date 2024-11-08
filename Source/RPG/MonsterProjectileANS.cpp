// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterProjectileANS.h"
#include "MonsterProjectile.h"
#include "Engine/World.h"
#include "Monster.h"

void UMonsterProjectileANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (Animation) {
			SpawnActorAt(MeshComp);
		}
	}
}

void UMonsterProjectileANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (Animation) {
			SpawnActorAt(MeshComp);
		}
	}
}

void UMonsterProjectileANS::SpawnActorAt(USkeletalMeshComponent* MeshComp)
{
	if (MeshComp && MeshComp->GetOwner()) {
		FVector SpawnLocation = MeshComp->GetSocketLocation(FName("MonsterProjectile")) + FVector(100.f, 100.f, 100.f);
		if (AMonsterProjectileClass) {
			if (auto* MonsterProjectile = MeshComp->GetWorld()->SpawnActor<AMonsterProjectile>(AMonsterProjectileClass, SpawnLocation, FRotator::ZeroRotator)) {
				AActor* OwnerActor = MeshComp->GetOwner();
				MonsterProjectile->SetOwner(OwnerActor);
				MonsterProjectile->SetInstigator(Cast<APawn>(OwnerActor));
				MonsterProjectile->DamageAmount = 10.f;
			}
		}
	}
}
