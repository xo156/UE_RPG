// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "BossMonster.h"

AMyGameModeBase::AMyGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MonsterDeadCount = 0;
}

void AMyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MonsterDeadCount == 10) {
		SpawnBossMonster(SpawnPoint);
		MonsterDeadCount++;

		FVector CurrentLocation = MoveableWall->GetActorLocation();
		FVector GoalLocation = CurrentLocation - FVector(CurrentLocation.X, CurrentLocation.Y, -CurrentLocation.Z * 2);

		FVector NewLocation = FMath::VInterpTo(CurrentLocation, GoalLocation, DeltaTime, 100.f);
		MoveableWall->SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, GoalLocation) < 1.0f) {
			MoveableWall->Destroy();
		}
	}
}

void AMyGameModeBase::SpawnBossMonster(AActor* SpawnPointActor)
{
	if (BossMonsterClass && SpawnPointActor) {
		FVector SpawnLocation = SpawnPointActor->GetActorLocation();
		FRotator SpawnRotation = SpawnPointActor->GetActorRotation();

		auto* BossMonster = GetWorld()->SpawnActor<ABossMonster>(BossMonsterClass, SpawnLocation, SpawnRotation);
		SpawnPointActor->Destroy();
	}
}
