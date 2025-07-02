// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumeStaminaAN.h"
#include "MyCharacter.h"
#include "StaminaActorComponent.h"

void UConsumeStaminaAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp || !MeshComp->GetOwner())
		return;

	if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		if (PlayerCharacter->GetStaminaActorComponent()) {
			PlayerCharacter->GetStaminaActorComponent()->ConsumeStamina(PlayerCharacter->AttackStaminaCost);
		}
	}
}
