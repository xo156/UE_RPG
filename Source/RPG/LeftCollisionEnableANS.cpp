// Fill out your copyright notice in the Description page of Project Settings.


#include "LeftCollisionEnableANS.h"
#include "MyCharacter.h"
#include "Weapon.h"
#include "WeaponBaseComponent.h"
#include "Components/BoxComponent.h"

void ULeftCollisionEnableANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
			PlayerCharacter->GetCurrentWeapon()->GetLeftHandWeaponInstance()->WeaponCollision->SetCollisionProfileName("Weapon");
			PlayerCharacter->GetCurrentWeapon()->GetLeftHandWeaponInstance()->WeaponCollision->SetNotifyRigidBodyCollision(true);
			PlayerCharacter->GetCurrentWeapon()->LeftHandWeaponInstance->bHasHit = false;
		}
	}
}

void ULeftCollisionEnableANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
			PlayerCharacter->GetCurrentWeapon()->GetLeftHandWeaponInstance()->WeaponCollision->SetCollisionProfileName("NoCollision");
			PlayerCharacter->GetCurrentWeapon()->GetLeftHandWeaponInstance()->WeaponCollision->SetNotifyRigidBodyCollision(false);
			PlayerCharacter->GetCurrentWeapon()->GetLeftHandWeaponInstance()->HitMonsters.Empty();
		}
	}
}
