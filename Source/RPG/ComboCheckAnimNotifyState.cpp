// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboCheckAnimNotifyState.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"

void UComboCheckAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner())) {
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(Character->GetController())) {
			PlayerController->bIsAttacking = true;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("NotifyBegin"));
		}
	}
}

void UComboCheckAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner())) {
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(Character->GetController())) {
			PlayerController->bIsAttacking = false;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("NotifyEnd"));
		}
	}
}
