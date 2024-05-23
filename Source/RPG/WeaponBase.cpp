// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "MyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AWeaponBase::AWeaponBase(const class FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // RootComponent를 따로 만들어서 여기로 붙여버리기
    USceneComponent* Root = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
    RootComponent = Root;

    //오른손
    WeaponRightHandMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponRightHandMesh"));
    WeaponRightHandMesh->SetupAttachment(RootComponent);
    WeaponRightHandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponRightCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponRightCollision"));
    WeaponRightCollision->SetBoxExtent(FVector(3.f, 3.f, 3.f));
    WeaponRightCollision->AttachToComponent(WeaponRightHandMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "Socket_R");

    //왼손
    WeaponLeftHandMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponLeftHandMesh"));
    WeaponLeftHandMesh->SetupAttachment(RootComponent);
    WeaponLeftHandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponLeftCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponLeftCollision"));
    WeaponLeftCollision->SetBoxExtent(FVector(3.f, 3.f, 3.f));
    WeaponLeftCollision->AttachToComponent(WeaponRightHandMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "Socket_L");

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
    if (MyCharacter != NewOwner)
        MyCharacter = NewOwner;
}

void AWeaponBase::AttachMeshToCharacter()
{
    if (MyCharacter) {
        USkeletalMeshComponent* CharacterMesh = MyCharacter->GetSpecificMesh();
        FName AttachPoint = MyCharacter->GetWeaponAttachPoint();
        //WeaponLeftHandMesh->AttachToComponent()
    }
}

void AWeaponBase::OnEquip(const AWeaponBase* LastWeapon)
{
}
