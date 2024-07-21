// Fill out your copyright notice in the Description page of Project Settings.


#include "LeftCollisionEnableANS.h"
#include "MyCharacter.h"
#include "Weapon.h"
#include "WeaponBaseComponent.h"
#include "Components/BoxComponent.h"

void ULeftCollisionEnableANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (MeshComp && MeshComp->GetOwner()) {
        if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
            if (auto* WeaponInstance = PlayerCharacter->GetCurrentWeapon()->GetLeftHandWeaponInstance()) {
                if (WeaponInstance->GetWeaponCollision()) {
                    WeaponInstance->GetWeaponCollision()->SetCollisionProfileName("Weapon");
                    WeaponInstance->GetWeaponCollision()->SetNotifyRigidBodyCollision(true);
                    UE_LOG(LogTemp, Warning, TEXT("Left Collision Enabled"));
                }
                else {
                    UE_LOG(LogTemp, Error, TEXT("WeaponCollision is null"));
                }
            }
            else {
                UE_LOG(LogTemp, Error, TEXT("WeaponInstance is null"));
            }
        }
    }
}

void ULeftCollisionEnableANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp && MeshComp->GetOwner()) {
        if (auto* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
            if (auto* WeaponInstance = PlayerCharacter->GetCurrentWeapon()->GetLeftHandWeaponInstance()) {
                if (WeaponInstance->GetWeaponCollision()) {
                    WeaponInstance->GetWeaponCollision()->SetCollisionProfileName("NoCollision");
                    WeaponInstance->GetWeaponCollision()->SetNotifyRigidBodyCollision(false);
                    UE_LOG(LogTemp, Warning, TEXT("Left Collision Disabled"));
                }
                else {
                    UE_LOG(LogTemp, Error, TEXT("WeaponCollision is null"));
                }
            }
            else {
                UE_LOG(LogTemp, Error, TEXT("WeaponInstance is null"));
            }
        }
    }
}
