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
	WeaponCollision->SetCollisionProfileName("NoCollision");
	WeaponCollision->SetNotifyRigidBodyCollision(false);
    WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // ���� �� ���� ��� Ȱ��ȭ

	// �浹 �̺�Ʈ ���ε�
	WeaponCollision->OnComponentHit.AddDynamic(this, &AWeapon::OnWeaponHit);
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

void AWeapon::OnWeaponHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    OwnerCharacter = Cast<AMyCharacter>(GetOwner());

    if (OwnerCharacter && OtherActor && (OtherActor != OwnerCharacter) && OtherComp) {
        if (!HitMonsters.Contains(OtherActor)) {
            HitMonsters.Add(OtherActor);
            UE_LOG(LogTemp, Warning, TEXT("Added %s to hit monsters list."), *OtherActor->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("%s is already in the hit monsters list."), *OtherActor->GetName());
        }

        if (HitMonsters.Num() > 0) {
            for (AActor* HitMonster : HitMonsters) {
                if (HitMonster->ActorHasTag("Enemy")) {
                    // �α׸� ���� �� ĳ���Ϳ� ���ظ� �����Ѵٰ� ǥ��
                    UE_LOG(LogTemp, Warning, TEXT("Applying damage to enemy: %s"), *HitMonster->GetName());

                    // HitMonster�� AMonster�� �ν��Ͻ����� Ȯ�� �� ĳ����
                    if (auto* Monster = Cast<AMonster>(HitMonster)) {
                        UE_LOG(LogTemp, Warning, TEXT("Monster Cast: %s"), *Monster->GetName());
                        // DamageEvent�� ����
                        FDamageEvent DamageEvent;

                        // Monster�� TakeDamage �޼��带 ȣ���Ͽ� ���ظ� ����
                        Monster->TakeDamage(OwnerCharacter->CharacterStatus.Damage, DamageEvent, OwnerCharacter->GetInstigatorController(), OwnerCharacter);
                    }
                    else {
                        // HitMonster�� AMonster �ν��Ͻ��� �ƴ� ��� �α� ���
                        UE_LOG(LogTemp, Warning, TEXT("%s is not a monster, skipping damage application."), *HitMonster->GetName());
                    }
                }
                else {
                    // HitMonster�� "Enemy" �±װ� ���� ��� �α� ���
                    UE_LOG(LogTemp, Warning, TEXT("%s is not an enemy, skipping damage application."), *HitMonster->GetName());
                }
            }
        }
        else {
            // HitMonsters ����Ʈ�� ��� �ִ� ��� �α� ���
            UE_LOG(LogTemp, Warning, TEXT("No hit monsters to apply damage to."));
        }
    }
}
