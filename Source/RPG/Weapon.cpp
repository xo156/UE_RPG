// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "MyCharacter.h"
#include "Components/BoxComponent.h"
#include "Monster.h"
#include "Kismet/GameplayStatics.h"

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

	// 충돌 이벤트 바인딩
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
	if (bHasHit)
		return;

	if (OtherActor && (OtherActor != this) && OtherComp) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("OtherActor, Weapon collided with: %s"), *OtherActor->GetName()));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HitComponent, Weapon collided with: %s"), *HitComponent->GetName()));
		

		if (!HitMonsters.Contains(OtherActor)) {
			HitMonsters.Add(OtherActor); //공격이 닿으면 리스트에 추가
		}
		//임시 확인용
		if (HitMonsters.Num() > 0) {
			for (AActor* TargetActor : HitMonsters) {
				UE_LOG(LogTemp, Warning, TEXT("Target is-----> %s"), *TargetActor->GetName());
			
			}
		}
		bHasHit = true;
	}
}
