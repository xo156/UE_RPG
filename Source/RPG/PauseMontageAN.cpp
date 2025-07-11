// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMontageAN.h"
#include "MyCharacter.h"

void UPauseMontageAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp || !MeshComp->GetOwner())
		return;

	auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner());
	if (!PlayerCharacter)
		return;

	auto* AnimInstance = MeshComp->GetAnimInstance();
	if (!AnimInstance)
		return;

	AnimInstance->Montage_Pause(AnimInstance->GetCurrentActiveMontage());
	PlayerCharacter->bIsChargingHeavyAttack = true;
	//PlayerCharacter->CurrentChargeTime = 0.f;
}