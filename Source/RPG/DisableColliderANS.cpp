// Fill out your copyright notice in the Description page of Project Settings.


#include "DisableColliderANS.h"
#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"

void UDisableColliderANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		//PlayerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	}
}

void UDisableColliderANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		PlayerCharacter->bIsRoll = false;
		//PlayerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(FName("Pawn"));
	}
}
