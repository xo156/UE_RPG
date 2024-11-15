// Fill out your copyright notice in the Description page of Project Settings.


#include "DieParticleNotify.h"
#include "Kismet/GameplayStatics.h"
#include "Monster.h"

void UDieParticleNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (DieParticleSystem) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DieParticleSystem, MeshComp->GetOwner()->GetActorLocation(), MeshComp->GetOwner()->GetActorRotation());
			if (auto* Monster = Cast<AMonster>(MeshComp->GetOwner())) {
				Monster->GetMesh()->SetVisibility(false);
			}
		}
	}
}
