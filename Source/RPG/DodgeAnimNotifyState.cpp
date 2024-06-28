// Fill out your copyright notice in the Description page of Project Settings.


#include "DodgeAnimNotifyState.h"
#include "MyCharacter.h"

void UDodgeAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (AMyCharacter* Character = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		Character->bIsDodging = true;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("NotifyBegin"));
	}
}

void UDodgeAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AMyCharacter* Character = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		Character->bIsDodging = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("NotifyBegin"));
	}
}
