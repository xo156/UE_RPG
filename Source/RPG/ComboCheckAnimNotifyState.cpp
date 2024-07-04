// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboCheckAnimNotifyState.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"

void UComboCheckAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (AMyCharacter* Character = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		Character->bIsAttack = true;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("NotifyBegin"));
	}
}

void UComboCheckAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AMyCharacter* Character = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		Character->bIsAttack = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("NotifyEnd"));
	}
}
