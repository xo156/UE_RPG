// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCollisionEnableANS.h"
#include "Monster.h"
#include "Components/CapsuleComponent.h"

void UMonsterCollisionEnableANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (auto* Monster = Cast<AMonster>(MeshComp->GetOwner())) {
			Monster->GetAttackCollision()->SetCollisionProfileName(FName("Enemy"));
			Monster->GetAttackCollision()->SetNotifyRigidBodyCollision(true);
		}
	}
}

void UMonsterCollisionEnableANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (auto* Monster = Cast<AMonster>(MeshComp->GetOwner())) {
			Monster->GetAttackCollision()->SetCollisionProfileName(FName("NoCollision"));
			Monster->GetAttackCollision()->SetNotifyRigidBodyCollision(false);
			Monster->GetOverlapActors().Empty();
		}
	}
}
