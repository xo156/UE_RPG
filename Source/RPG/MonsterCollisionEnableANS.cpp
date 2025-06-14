// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCollisionEnableANS.h"
#include "MonsterBase.h"
#include "Components/CapsuleComponent.h"

void UMonsterCollisionEnableANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (!MeshComp)
		return;

	if (auto* MonsterBase = Cast<AMonsterBase>(MeshComp->GetOwner())) {
		MonsterBase->EnableAttackBody(TargetBodyName, true);
	}
}

void UMonsterCollisionEnableANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
		return;

	if (auto* MonsterBase = Cast<AMonsterBase>(MeshComp->GetOwner())) {
		MonsterBase->EnableAttackBody(TargetBodyName, false);
	}
}
