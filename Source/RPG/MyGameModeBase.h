// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMyGameModeBase();

	virtual void Tick(float DeltaTime) override;

public:
	void SpawnBossMonster(AActor* SpawnPointActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<ABossMonster> BossMonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	AActor* SpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	AActor* MoveableWall;

	int32 MonsterDeadCount;
};
