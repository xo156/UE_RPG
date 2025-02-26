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
    if (!OwnerCharacter) return;

    auto AttachWeapon = [&](TSubclassOf<AWeapon> WeaponClass, FName SocketName, AWeapon*& WeaponInstance) {
        if (WeaponClass) {
            WeaponInstance = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
            if (WeaponInstance) {
                WeaponInstance->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
                WeaponInstance->SetOwnerCharacter(OwnerCharacter);
            }
        }
    };

    AttachWeapon(RightHandWeapon, TEXT("Socket_R"), RightHandWeaponInstance);
    AttachWeapon(LeftHandWeapon, TEXT("Socket_L"), LeftHandWeaponInstance);
}

AWeapon* UWeaponBaseComponent::GetRightHandWeaponInstance() const
{
    return RightHandWeaponInstance;
}

AWeapon* UWeaponBaseComponent::GetLeftHandWeaponInstance() const
{
    return LeftHandWeaponInstance;
}
