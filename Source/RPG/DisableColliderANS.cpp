// Fill out your copyright notice in the Description page of Project Settings.


#include "DisableColliderANS.h"
#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UDisableColliderANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		PlayerCharacter->bIsRoll = true;
	}
}

void UDisableColliderANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
		PlayerCharacter->bIsRoll = false;

		if (PlayerCharacter->bIsLockon && PlayerCharacter->GetCurrentTarget()) {
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PlayerCharacter->GetActorLocation(), PlayerCharacter->GetCurrentTarget()->GetActorLocation());
			LookAtRotation.Pitch -= PlayerCharacter->GetTargetHeightOffset();
			PlayerCharacter->GetController()->SetControlRotation(LookAtRotation);
		}
	}
}
