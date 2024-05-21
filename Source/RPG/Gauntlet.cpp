// Fill out your copyright notice in the Description page of Project Settings.


#include "Gauntlet.h"


AGauntlet::AGauntlet()
{
	MyWeapon.WeaponDamage = 100;
	MyWeapon.WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	RootComponent = MyWeapon.WeaponMesh;
	MyWeapon.WeaponType = FText::FromString(TEXT("Gauntlet"));
}

void AGauntlet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGauntlet::BeginPlay()
{
	Super::BeginPlay();
}

void AGauntlet::SetMontageLength()
{
	MontageLength.Emplace(AttackMontage1->GetPlayLength());
	MontageLength.Emplace(AttackMontage2->GetPlayLength());
	MontageLength.Emplace(AttackMontage3->GetPlayLength());
}
