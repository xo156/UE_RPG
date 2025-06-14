// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MonsterAttackPatternDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UMonsterAttackPatternDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float RequiredDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float RequiredHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Cooldown = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float DelayBeforeAttack = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	float LastAttackTime = -FLT_MAX; // 최근 사용 시간

	bool bIsValidCondition(float Distance, float HP);
};
