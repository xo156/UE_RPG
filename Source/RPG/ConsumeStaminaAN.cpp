// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumeStaminaAN.h"
#include "MyCharacter.h"

void UConsumeStaminaAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp || !MeshComp->GetOwner())
		return;

	if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		if (PlayerCharacter->GetResourceComponent()) {
			PlayerCharacter->GetResourceComponent()->ConsumeStamina(PlayerCharacter->AttackStaminaCost);
		}
	}
}
