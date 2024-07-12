// Fill out your copyright notice in the Description page of Project Settings.


#include "RightCollisionEnableANS.h"
#include "MyCharacter.h"
#include "Weapon.h"
#include "WeaponBaseComponent.h"
#include "Components/BoxComponent.h"

void URightCollisionEnableANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
			PlayerCharacter->GetCurrentWeapon()->GetRightHandWeaponInstance()->WeaponCollision->SetCollisionProfileName("Weapon");
			PlayerCharacter->GetCurrentWeapon()->GetRightHandWeaponInstance()->WeaponCollision->SetNotifyRigidBodyCollision(true);
			PlayerCharacter->GetCurrentWeapon()->RightHandWeaponInstance->bHasHit = false;
		}
	}
}

void URightCollisionEnableANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
			PlayerCharacter->GetCurrentWeapon()->GetRightHandWeaponInstance()->WeaponCollision->SetCollisionProfileName("NoCollision");
			PlayerCharacter->GetCurrentWeapon()->GetRightHandWeaponInstance()->WeaponCollision->SetNotifyRigidBodyCollision(false);
			PlayerCharacter->GetCurrentWeapon()->GetRightHandWeaponInstance()->HitMonsters.Empty();
		}
	}
}
