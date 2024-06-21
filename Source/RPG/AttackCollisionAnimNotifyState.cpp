// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCollisionAnimNotifyState.h"
#include "MyCharacter.h"
#include "Weapon.h"
#include "WeaponBaseComponent.h"
#include "Components/BoxComponent.h"

void UAttackCollisionAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
			PlayerCharacter->GetCurrentWeapon()->GetRightHandWeaponInstance()->WeaponCollision->SetCollisionProfileName("Weapon");
			PlayerCharacter->GetCurrentWeapon()->GetRightHandWeaponInstance()->WeaponCollision->SetNotifyRigidBodyCollision(true);
			PlayerCharacter->GetCurrentWeapon()->RightHandWeaponInstance->bHasHit = false;

			PlayerCharacter->GetCurrentWeapon()->GetLeftHandWeaponInstance()->WeaponCollision->SetCollisionProfileName("Weapon");
			PlayerCharacter->GetCurrentWeapon()->GetLeftHandWeaponInstance()->WeaponCollision->SetNotifyRigidBodyCollision(true);
			PlayerCharacter->GetCurrentWeapon()->LeftHandWeaponInstance->bHasHit = false;
		}
	}
}

void UAttackCollisionAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner())) {
			PlayerCharacter->GetCurrentWeapon()->GetRightHandWeaponInstance()->WeaponCollision->SetCollisionProfileName("NoCollision");
			PlayerCharacter->GetCurrentWeapon()->GetRightHandWeaponInstance()->WeaponCollision->SetNotifyRigidBodyCollision(false);
			PlayerCharacter->GetCurrentWeapon()->GetRightHandWeaponInstance()->HitMonsters.Empty();

			PlayerCharacter->GetCurrentWeapon()->GetLeftHandWeaponInstance()->WeaponCollision->SetCollisionProfileName("NoCollision");
			PlayerCharacter->GetCurrentWeapon()->GetLeftHandWeaponInstance()->WeaponCollision->SetNotifyRigidBodyCollision(false);
			PlayerCharacter->GetCurrentWeapon()->GetLeftHandWeaponInstance()->HitMonsters.Empty();

			if (PlayerCharacter->GetCurrentWeapon()->GetRightHandWeaponInstance()->HitMonsters.Num() == 0) {
				UE_LOG(LogTemp, Error, TEXT("HitMonsters Empty In RightHandWeaponInstance"));
			}
			if (PlayerCharacter->GetCurrentWeapon()->GetLeftHandWeaponInstance()->HitMonsters.Num() == 0) {
				UE_LOG(LogTemp, Error, TEXT("HitMonsters Empty In LeftHandWeaponInstance"));
			}
		}
	}
}
