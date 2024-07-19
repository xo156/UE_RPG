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
                if (WeaponInstance->WeaponCollision) {
                    WeaponInstance->WeaponCollision->SetCollisionProfileName("Weapon");
                    WeaponInstance->WeaponCollision->SetNotifyRigidBodyCollision(true);
                    UE_LOG(LogTemp, Warning, TEXT("Collision Enabled"));
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
                if (WeaponInstance->WeaponCollision) {
                    WeaponInstance->WeaponCollision->SetCollisionProfileName("NoCollision");
                    WeaponInstance->WeaponCollision->SetNotifyRigidBodyCollision(false);
                    WeaponInstance->HitMonsters.Empty();
                    UE_LOG(LogTemp, Warning, TEXT("Collision Disabled"));
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
