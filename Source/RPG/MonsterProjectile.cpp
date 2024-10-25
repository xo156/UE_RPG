// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterProjectile.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/DamageType.h"
#include "Monster.h"

// Sets default values
AMonsterProjectile::AMonsterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //面倒且 备眉 积己
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    RootComponent = SphereComponent;
    SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));
    SphereComponent->OnComponentHit.AddDynamic(this, &AMonsterProjectile::OnHit);

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(RootComponent);
    ProjectileMesh->SetCollisionProfileName(TEXT("NoCollision"));
    ProjectileMesh->SetVisibility(true);

}

// Called when the game starts or when spawned
void AMonsterProjectile::BeginPlay()
{
	Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(ProjectileTimerHandle, this, &AMonsterProjectile::DestroyProjectile, ProjectileLifeTime, false);

    FVector ForwardPosition = GetActorLocation() + GetActorForwardVector() * 200.f;
    SetActorLocation(ForwardPosition);
}

// Called every frame
void AMonsterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    GoToPlayer();
}

void AMonsterProjectile::GoToPlayer()
{
    if (auto* PlayerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
        FVector DirectionToPlayer = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        ProjectileSpeed = (GetDistanceTo(PlayerCharacter) / ProjectileLifeTime);

        FVector NewLocation = GetActorLocation() + (DirectionToPlayer * ProjectileSpeed * GetWorld()->DeltaTimeSeconds);
        SetActorLocation(NewLocation);
    }
}

void AMonsterProjectile::DestroyProjectile()
{
    Destroy();
}

void AMonsterProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this && OtherActor->IsA(AMyCharacter::StaticClass())) {
        if (Monster) {
            float DamageAmount = Monster->MonsterStatus.Damage;
            TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass();

            UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, nullptr, this, DamageType);
            Destroy();
        }
    }
}

