// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MonsterBase.h"
#include "MonsterAttackPatternDataAsset.h"
#include "MonsterData.generated.h"

USTRUCT(BlueprintType)
struct RPG_API FMonsterData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 MonsterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName MonsterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<AMonsterBase> MonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxMonsterHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<int32> DropItemIDS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<UMonsterAttackPatternDataAsset*> MonsterAttackPatterns;
};
