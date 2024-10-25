// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterProjectileANS.h"
#include "MonsterProjectile.h"
#include "Engine/World.h"

void UMonsterProjectileANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (Animation) {
			float Duration = Animation->GetPlayLength();
			SpawnActorAt(MeshComp, 0.f, Duration);
		}
	}
}

void UMonsterProjectileANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (Animation) {
			float Duration = Animation->GetPlayLength();
			SpawnActorAt(MeshComp, Duration / 2, Duration);
			SpawnActorAt(MeshComp, Duration, Duration);
		}
	}
}

void UMonsterProjectileANS::SpawnActorAt(USkeletalMeshComponent* MeshComp, float Time, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner()) {
		FVector SpawnLocation = MeshComp->GetSocketLocation(FName("MonsterProjectile")) + FVector(0, 0, 100.f);
		if (AMonsterProjectileClass) {
			AMonsterProjectile* MonsterProjectile = MeshComp->GetWorld()->SpawnActor<AMonsterProjectile>(AMonsterProjectileClass, SpawnLocation, FRotator::ZeroRotator);
		}
	}
}
