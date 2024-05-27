// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBaseComponent.h"
#include "MyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

//// Sets default values
//UWeaponBaseComponent::UWeaponBaseComponent()
//{
//    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
//    // off to improve performance if you don't need them.    PrimaryComponentTick.bCanEverTick = false;
//
//    
//}

// Called every frame
void UWeaponBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

// Called when the game starts or when spawned
void UWeaponBaseComponent::BeginPlay()
{
	Super::BeginPlay();
	
    // RootComponent 설정
    if (USceneComponent* ParentRoot = GetOwner()->GetRootComponent()) {
        // 오른쪽 손에 대한 루트 생성 및 부착
        WeaponRightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponRightHandMesh"));
        WeaponRightHandMesh->SetupAttachment(ParentRoot, "Socket_R");
        WeaponRightHandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        WeaponRightCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponRightCollision"));
        WeaponRightCollision->SetBoxExtent(FVector(3.f, 3.f, 3.f));
        WeaponRightCollision->SetupAttachment(WeaponRightHandMesh, "Socket_R");

        // 왼쪽 손에 대한 루트 생성 및 부착
        WeaponLeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponLeftHandMesh"));
        WeaponLeftHandMesh->SetupAttachment(ParentRoot, "Socket_L");
        WeaponLeftHandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        WeaponLeftCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponLeftCollision"));
        WeaponLeftCollision->SetBoxExtent(FVector(3.f, 3.f, 3.f));
        WeaponLeftCollision->SetupAttachment(WeaponLeftHandMesh, "Socket_L");
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Parent actor of UWeaponBaseComponent does not have a root component!"));
    }
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
        AttachMeshToCharacter();
    }
}

void UWeaponBaseComponent::AttachMeshToCharacter()
{
    if (MyCharacter) {
        if (USkeletalMeshComponent* CharacterMesh = MyCharacter->GetMesh()) {
            WeaponRightHandMesh->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Socket_R"));
            WeaponLeftHandMesh->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Socket_L"));

            //잘 붙는지 확인용
            if (WeaponRightHandMesh->GetAttachParent() == CharacterMesh) {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("RightHandMesh successfully attached to Socket_R"));
            }
            else  {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to attach RightHandMesh to Socket_R"));
            }

            if (WeaponLeftHandMesh->GetAttachParent() == CharacterMesh) {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("LeftHandMesh successfully attached to Socket_L"));
            }
            else  {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to attach LeftHandMesh to Socket_L"));
            }
        }
    }
}