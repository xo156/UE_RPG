// Fill out your copyright notice in the Description page of Project Settings.


#include "DisableColliderANS.h"
#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"

void UDisableColliderANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		PlayerCharacter->bIsDodge = true;
		PlayerCharacter->GetCapsuleComponent()->SetActive(false);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("invincibility Start"));
	}
}

void UDisableColliderANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		PlayerCharacter->bIsDodge = false;
		PlayerCharacter->GetCapsuleComponent()->SetActive(true);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("invincibility End"));
	}
}
