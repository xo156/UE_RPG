// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboCheckANS.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"

void UComboCheckANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (AMyCharacter* Character = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		Character->bIsAttack = true;
	}
}

void UComboCheckANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AMyCharacter* Character = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		Character->bIsAttack = false;
	}
}
