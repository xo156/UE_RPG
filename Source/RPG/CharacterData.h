// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct RPG_API FCharacterData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxCharacterHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxCharacterStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CharacterDamage;
};
