// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "MyCharacter.h"
#include "Components/BoxComponent.h"
#include "Monster.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

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
	WeaponCollision->SetCollisionProfileName("Weapon");
	WeaponCollision->SetNotifyRigidBodyCollision(true);
    WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 물리 및 쿼리 모두 활성화


	// 충돌 이벤트 바인딩
	WeaponCollision->OnComponentHit.AddDynamic(this, &AWeapon::OnWeaponAttackHit);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	HitMonsters.Empty();
	OwnerCharacter = Cast<AMyCharacter>(GetOwner());
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::OnWeaponAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Validate if the owner character is valid
    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("OwnerCharacter is null. Cannot process hit."));
        return;
    }

    // Validate hit and other actor
    if (OtherActor == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("OtherActor is null. No collision detected."));
        return;
    }

    if (OtherComp == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("OtherComp is null. No valid component found for the hit."));
        return;
    }

    if (OtherActor == OwnerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon hit the owner character, skipping damage application."));
        return;
    }

    // Log details about the hit
    UE_LOG(LogTemp, Warning, TEXT("Weapon collided with: %s"), *OtherActor->GetName());
    UE_LOG(LogTemp, Warning, TEXT("HitComponent details: %s"), *HitComponent->GetName());

    // Add the actor to the hit monsters list if not already present
    if (!HitMonsters.Contains(OtherActor))
    {
        HitMonsters.Add(OtherActor);
        UE_LOG(LogTemp, Warning, TEXT("Added %s to hit monsters list."), *OtherActor->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s is already in the hit monsters list."), *OtherActor->GetName());
    }

    // Iterate over all hit monsters and apply damage if they are enemies
    if (HitMonsters.Num() > 0)
    {
        for (AActor* HitMonster : HitMonsters)
        {
            if (HitMonster->ActorHasTag("Enemy"))
            {
                UE_LOG(LogTemp, Warning, TEXT("Applying damage to enemy: %s"), *HitMonster->GetName());

                FDamageEvent DamageEvent;
                float ActualDamage = HitMonster->TakeDamage(OwnerCharacter->CharacterStatus.Damage, DamageEvent, OwnerCharacter->GetInstigatorController(), OwnerCharacter);

                // Log the actual damage applied
                UE_LOG(LogTemp, Warning, TEXT("Damage applied to %s: %f"), *HitMonster->GetName(), ActualDamage);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("%s is not an enemy, skipping damage application."), *HitMonster->GetName());
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No hit monsters to apply damage to."));
    }

}
