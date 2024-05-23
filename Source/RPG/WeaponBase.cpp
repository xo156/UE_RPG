// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AWeaponBase::AWeaponBase(const class FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // RootComponent 설정
    USceneComponent* Root = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
    RootComponent = Root;

    //오른손 실제로 붙이기
    WeaponRightHandMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponRightHandMesh"));
    WeaponRightHandMesh->SetupAttachment(Root, "Socket_R");
    WeaponRightHandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    WeaponRightCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponRightCollision"));
    WeaponRightCollision->SetBoxExtent(FVector(3.f, 3.f, 3.f));
    WeaponRightCollision->SetupAttachment(WeaponRightHandMesh, "Socket_R");

    //왼손 실제로 붙이기
    WeaponLeftHandMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponLeftHandMesh"));
    WeaponLeftHandMesh->SetupAttachment(Root, "Socket_L");
    WeaponLeftHandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    WeaponLeftCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponLeftCollision"));
    WeaponLeftCollision->SetBoxExtent(FVector(3.f, 3.f, 3.f));
    WeaponLeftCollision->SetupAttachment(WeaponLeftHandMesh, "Socket_L");
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	
}

int AWeaponBase::GetSectionCount(UAnimMontage* Montage)
{
	if(Montage)
		return Montage->CompositeSections.Num();
	return 0;
}

void AWeaponBase::SetOwnerCharacter(AMyCharacter* NewOwner)
{
    //캐릭터는 하나니까
    if (MyCharacter != NewOwner) {
        MyCharacter = NewOwner;
        AttachMeshToCharacter();
    }
}

void AWeaponBase::AttachMeshToCharacter()
{
    if (MyCharacter) {
        USkeletalMeshComponent* CharacterMesh = MyCharacter->GetMesh();
        if (CharacterMesh) {
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