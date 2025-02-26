// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboCheckANS.h"
#include "MyCharacter.h"
#include "WeaponBaseComponent.h"
#include "StateMachineComponent.h"

void UComboCheckANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (!MeshComp || !MeshComp->GetOwner())
		return;

	if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		UE_LOG(LogTemp, Log, TEXT("ComboCheckANS::NotifyBegin"));
		PlayerCharacter->bIsEnableCombo = true;
		PlayerCharacter->SetNextSectionName(NextSection);
	}
}

void UComboCheckANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp || !MeshComp->GetOwner())
		return;

	if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		UE_LOG(LogTemp, Log, TEXT("ComboCheckANS::NotifyEnd"));
		PlayerCharacter->bIsEnableCombo = false;
	}
}
