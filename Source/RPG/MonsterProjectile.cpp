// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterProjectile.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/DamageType.h"
#include "Monster.h"
#include "Engine/DamageEvents.h"

// Sets default values
AMonsterProjectile::AMonsterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //충돌할 구체 생성
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    RootComponent = SphereComponent;
    SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonsterProjectile::OnBeginOverlap);

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(RootComponent);
    ProjectileMesh->SetCollisionProfileName(TEXT("NoCollision"));
    ProjectileMesh->SetVisibility(true);

}

// Called when the game starts or when spawned
void AMonsterProjectile::BeginPlay()
{
	Super::BeginPlay();
    
    //TODO: AMonster와 ABossMonster를 사용할 수 있도록 캐스팅
    //ABossMonster는 AMonster의 자식 클래스
    if (MonsterActor) {
        if (auto* Monster = Cast<AMonster>(MonsterActor)) {
            DamageAmount = Monster->MonsterStatus.Damage;
            InstigatorController = Monster->GetController();

        }
    }


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
    Destroy();
}

void AMonsterProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("AMonsterProjectile::OnBeginOverlap"));
    if (OtherActor && OtherActor != this && OtherActor->IsA(AMyCharacter::StaticClass())) {
        FDamageEvent DamageEvent;
        OtherActor->TakeDamage(DamageAmount, DamageEvent, InstigatorController, this);
        Destroy();
    }
}

