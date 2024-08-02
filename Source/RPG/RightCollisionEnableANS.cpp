// Fill out your copyright notice in the Description page of Project Settings.


#include "RightCollisionEnableANS.h"
#include "MyCharacter.h"
#include "Weapon.h"
#include "WeaponBaseComponent.h"
#include "Components/BoxComponent.h"

void URightCollisionEnableANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (MeshComp && MeshComp->GetOwner()) {
        if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
            if (auto* WeaponInstance = PlayerCharacter->GetCurrentWeapon()->GetRightHandWeaponInstance()) {
                if (WeaponInstance->GetWeaponCollision()) {
                    WeaponInstance->GetWeaponCollision()->SetCollisionProfileName("Weapon");
                    WeaponInstance->GetWeaponCollision()->SetNotifyRigidBodyCollision(true);
                }
            }
        }
    }
}

void URightCollisionEnableANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp && MeshComp->GetOwner()) {
        if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
            if (auto* WeaponInstance = PlayerCharacter->GetCurrentWeapon()->GetRightHandWeaponInstance()) {
                if (WeaponInstance->GetWeaponCollision()) {
                    WeaponInstance->GetWeaponCollision()->SetCollisionProfileName("NoCollision");
                    WeaponInstance->GetWeaponCollision()->SetNotifyRigidBodyCollision(false);
                }
            }
        }
    }
}
