// Fill out your copyright notice in the Description page of Project Settings.


#include "DisableColliderANS.h"
#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UDisableColliderANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		PlayerCharacter->bIsNoDamage = true;
	}
}

void UDisableColliderANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		PlayerCharacter->bIsRoll = false;
		PlayerCharacter->bIsNoDamage = false;
	}
}
