// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "MyCharacter.h"
#include "Components/BoxComponent.h"
#include "MonsterBase.h"
#include "HPActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "MyPlayerController.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = Root;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(WeaponMesh);
	WeaponCollision->SetHiddenInGame(false);
	WeaponCollision->SetVisibility(true);
	WeaponCollision->SetCollisionProfileName("NoCollision");
	WeaponCollision->SetNotifyRigidBodyCollision(false);
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && !OtherActor->IsA(AMyCharacter::StaticClass())) {
		if (OtherActor->ActorHasTag(FName("Enemy"))) {
			if (!OverlapActors.Contains(OtherActor)) {
				OverlapActors.Add(OtherActor);
				ApplyDamageToActor(OtherActor);
			}
		}
	}
}

void AWeapon::ApplyDamageToActor(AActor* ActorToDamage)
{
	if (OwnerCharacter == nullptr) {
		return;
	}

	//float Damage = OwnerCharacter->GetHPActorComponent()->GetCurrentDamage();
	float Damage = 0.f;
	FDamageEvent DamageEvent;
	if (GetInstigator() == nullptr) {
		return;
	}
	else {
		ActorToDamage->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
	}
}

USkeletalMeshComponent* AWeapon::GetWeaponMesh() const
{
	return WeaponMesh ? WeaponMesh : nullptr;
}

UBoxComponent* AWeapon::GetWeaponCollision() const
{
	return WeaponCollision ? WeaponCollision : nullptr;
}

FTransform AWeapon::GetHandOffsetTransform(EEquipSlot Slot) const
{
	if (Slot == EEquipSlot::LeftHand) {
		return FTransform(LeftHandRotationOffset, LeftHandLocationOffset);
	}
	else {
		return FTransform(RightHandRotationOffset, RightHandLocationOffset);
	}
}

TArray<AActor*>& AWeapon::GetOverlapActors()
{
	return OverlapActors;
}

UAnimMontage* AWeapon::GetLightAttackMontage() const
{
	return LightAttackMontage ? LightAttackMontage : nullptr;
}

UAnimMontage* AWeapon::GetHeavyAttackMontage() const
{
	return HeavyAttackMontage ? HeavyAttackMontage : nullptr;
}
