// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "MyCharacter.h"
#include "Components/BoxComponent.h"
#include "Monster.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "WeaponBaseComponent.h"

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
	WeaponCollision->SetupAttachment(RootComponent);

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
				if (OwnerCharacter && OwnerCharacter->bIsGuard) {
					ZeroDamageToOnwer();
				}
				else {
					ApplyDamageToActor(OtherActor);
				}
			}
		}
	}
}

void AWeapon::ApplyDamageToActor(AActor* ActorToDamage)
{
	if (OwnerCharacter == nullptr) {
		return;
	}

	float Damage = OwnerCharacter->CharacterStatus.Damage;
	FDamageEvent DamageEvent;
	if (GetInstigator() == nullptr) {
		return;
	}
	else {
		ActorToDamage->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
	}
}

void AWeapon::ZeroDamageToOnwer()
{
	if (OwnerCharacter == nullptr) {
		return;
	}

	float Damage = 0.f;
	FDamageEvent DamageEvent;
	if (GetInstigator() == nullptr) {
		return;
	}
	else {
		OwnerCharacter->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
		OwnerCharacter->ConsumeStaminaForAction(OwnerCharacter->GuardStaminaCost);
		OwnerCharacter->bIsGuard = false;
		UE_LOG(LogTemp, Log, TEXT("Guard Success"));
	}
}

void AWeapon::SetOwnerCharacter(AMyCharacter* NewOwnerCharacter)
{
	if (NewOwnerCharacter) {
		OwnerCharacter = NewOwnerCharacter;
		SetInstigator(NewOwnerCharacter->GetController()->GetPawn());
		UE_LOG(LogTemp, Log, TEXT("OwnerCharacter set to: %s"), *OwnerCharacter->GetName());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("SetOwnerCharacter called with null"));
	}
}

void AWeapon::Use()
{
	UE_LOG(LogTemp, Log, TEXT("AWeapon::Use()"));
	if (auto* Player = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
		Player->EquipWeapon(ThisWeaponComponent);
	}

}

USkeletalMeshComponent* AWeapon::GetWeaponMesh() const
{
	return WeaponMesh;
}

UBoxComponent* AWeapon::GetWeaponCollision() const
{
	return WeaponCollision;
}

TArray<AActor*>& AWeapon::GetOverlapActors()
{
	return OverlapActors;
}
