// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBaseComponent.h"
#include "MyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Weapon.h"

//// Sets default values
UWeaponBaseComponent::UWeaponBaseComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bCanEverTick = false;


}

// Called every frame
void UWeaponBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
}

// Called when the game starts or when spawned
void UWeaponBaseComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

int UWeaponBaseComponent::GetSectionCount(UAnimMontage* Montage)
{
	if(Montage)
		return Montage->CompositeSections.Num();
	return 0;
}

void UWeaponBaseComponent::SetOwnerCharacter(AMyCharacter* NewOwner)
{
    //캐릭터는 하나니까
    if (OwnerCharacter != NewOwner) {
        OwnerCharacter = NewOwner;
        AttachToCharacter();
    }
}

void UWeaponBaseComponent::AttachToCharacter()
{
    if (OwnerCharacter) {
        if (RightHandWeapon) {
            if (RightHandWeaponInstance = GetWorld()->SpawnActor<AWeapon>(RightHandWeapon)) {
                RightHandWeaponInstance->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("Socket_R")));
                RightHandWeaponInstance->SetOwnerCharacter(OwnerCharacter);
                OwnerCharacter->CharacterStatus.Damage += RightHandWeaponInstance->ItemData.ItemValue;
            }
        }
        if (LeftHandWeapon) {
            if (LeftHandWeaponInstance = GetWorld()->SpawnActor<AWeapon>(LeftHandWeapon)) {
                LeftHandWeaponInstance->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("Socket_L")));
                LeftHandWeaponInstance->SetOwnerCharacter(OwnerCharacter);
                OwnerCharacter->CharacterStatus.Damage += LeftHandWeaponInstance->ItemData.ItemValue;
            }
        }
    }
}

AWeapon* UWeaponBaseComponent::GetRightHandWeaponInstance() const
{
    if (RightHandWeaponInstance)
        return RightHandWeaponInstance;
    return nullptr;
}

AWeapon* UWeaponBaseComponent::GetLeftHandWeaponInstance() const
{
    if (LeftHandWeaponInstance)
        return LeftHandWeaponInstance;
    return nullptr;
}
