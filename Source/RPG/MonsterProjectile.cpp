// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterProjectile.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMonsterProjectile::AMonsterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMonsterProjectile::BeginPlay()
{
	Super::BeginPlay();

    FVector ForwardPosition = GetActorLocation() + GetActorForwardVector() * 200.f;
    SetActorLocation(ForwardPosition);

    GetWorldTimerManager().SetTimer(TimerHandle, this, &AMonsterProjectile::StartHomingToPlayer, 0.2f, false);
}

// Called every frame
void AMonsterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonsterProjectile::StartHomingToPlayer()
{
    if (auto* PlayerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
        FVector DirectionToPlayer = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        ProjectileMovementComponent->Velocity = DirectionToPlayer * ProjectileSpeed;
        ProjectileMovementComponent->bIsHomingProjectile = true;
        ProjectileMovementComponent->HomingTargetComponent = PlayerCharacter->GetRootComponent();
    }
}

