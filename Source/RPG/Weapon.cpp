// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "MyCharacter.h"
#include "Components/BoxComponent.h"
#include "NormalMonster.h"

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

	// �浹 �̺�Ʈ ���ε�
	WeaponCollision->OnComponentHit.AddDynamic(this, &AWeapon::OnWeaponAttackHit);
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

void AWeapon::OnWeaponAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && (OtherActor != this) && OtherComp && !bHasHit) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("OtherActor, Weapon collided with: %s"), *OtherActor->GetName()));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HitComponent, Weapon collided with: %s"), *HitComponent->GetName()));
	
		HitMonsters.Add(OtherActor); //������ ������ ����Ʈ�� �߰�
		for (AActor* HitMonster : HitMonsters) {
			//TODO: �÷��̾��� ������ ���� ���͵��� �����ð����� �����ǵ���
			
		}
	}
	bHasHit = true;
}

