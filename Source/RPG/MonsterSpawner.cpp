// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawner.h"
#include "MonsterData.h"
#include "MonsterBase.h"

// Sets default values
AMonsterSpawner::AMonsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnMonsterByID();
}

// Called every frame
void AMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonsterSpawner::SpawnMonsterByID()
{
	if (!MonsterDataTable) {
		UE_LOG(LogTemp, Warning, TEXT("MonsterDataTable is not set in MonsterSpawner"));
		return;
	}

	/*static const FString ContextString(TEXT("MonsterDataContext"));
	FMonsterData* MonsterData = MonsterDataTable->FindRow<FMonsterData>(FName(*FString::FromInt(MonsterIDToSpawn)), ContextString);

	if (MonsterData && MonsterData->MonsterClass) {
		UWorld* World = GetWorld();
		if (!World) return;

		FActorSpawnParameters SpawnParams;
		AMonsterBase* SpawnedMonster = World->SpawnActor<AMonsterBase>(
			MonsterData->MonsterClass,
			GetActorLocation(),
			GetActorRotation(),
			SpawnParams);

		if (SpawnedMonster) {
			SpawnedMonster->InitMonsterInfo(*MonsterData);
			UE_LOG(LogTemp, Log, TEXT("Monster spawned at %s with ID %d"), *GetActorLocation().ToString(), MonsterIDToSpawn);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid MonsterID or MonsterClass is null: ID %d"), MonsterIDToSpawn);
	}*/
}

