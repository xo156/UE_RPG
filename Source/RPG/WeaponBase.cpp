// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	RootComponent =WeaponMesh;
	WeaponDamage = 100;
	WeaponType = FText::FromString(TEXT("Gauntlet"));
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

void AWeaponBase::SetMontageLength()
{
	MontageLength.Emplace(AttackMontage1->GetPlayLength());
	MontageLength.Emplace(AttackMontage2->GetPlayLength());
	MontageLength.Emplace(AttackMontage3->GetPlayLength());
}

