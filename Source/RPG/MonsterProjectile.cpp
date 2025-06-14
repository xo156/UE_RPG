// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterProjectile.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/DamageType.h"
#include "MonsterBase.h"
#include "Engine/DamageEvents.h"

// Sets default values
AMonsterProjectile::AMonsterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    //충돌할 구체 생성
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = CollisionComponent;
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonsterProjectile::OnOverlapBegin);
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
        ProjectileSpeed = FMath::FInterpTo(ProjectileSpeed, TargetSpeed, GetWorld()->DeltaTimeSeconds, SpeedIncreaseRate);
        FVector NewLocation = GetActorLocation() + (DirectionToPlayer * ProjectileSpeed * GetWorld()->DeltaTimeSeconds);
        SetActorLocation(NewLocation);
    }

}

void AMonsterProjectile::DestroyProjectile()
{
    //시간 되면 사라지게 하기 위함
    Destroy();
}

void AMonsterProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherActor != GetOwner()) {
        if (OtherActor->IsA(AMyCharacter::StaticClass())) {
            if (auto* PlayerCharacter = Cast<AMyCharacter>(OtherActor)) {
                if (PlayerCharacter->bIsGuard) {
                    UE_LOG(LogTemp, Log, TEXT("Player is guarding, no damage dealt."));
                    Destroy();
                    return;
                }
                for (auto* CheckComponent : PlayerCharacter->GetComponents()) {
                    if (CheckComponent->ComponentHasTag(FName("Hitted"))) {
                        FDamageEvent DamageEvent;
                        PlayerCharacter->TakeDamage(DamageAmount, DamageEvent, GetInstigatorController(), this);
                        UE_LOG(LogTemp, Log, TEXT("Projectile hit player and dealt damage."));
                        Destroy();
                    }
                }
            }
        }
        else {
            Destroy();
        }
    }
}
