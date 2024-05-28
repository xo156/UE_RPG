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
    if (MyCharacter != NewOwner) {
        MyCharacter = NewOwner;
        AttachToCharacter();
    }
}

void UWeaponBaseComponent::AttachToCharacter()
{
    if (MyCharacter) {
        if (RightHandWeapon) {
            Weapon = GetWorld()->SpawnActor<AWeapon>(RightHandWeapon);
            Weapon->AttachToComponent(MyCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("Socket_R")));
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("RightWeaponAttach"));
        }
        if (LeftHandWeapon) {
            Weapon = GetWorld()->SpawnActor<AWeapon>(LeftHandWeapon);
            Weapon->AttachToComponent(MyCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("Socket_L")));
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("LeftWeaponAttach"));
        }
    }
}