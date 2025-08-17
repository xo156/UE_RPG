// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "EquipSlotType.h"
#include "EquipableItemData.generated.h"

USTRUCT(BlueprintType)
struct RPG_API FEquipableItemData : public FItemData
{
	GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackPower = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DefensePower = 0.f;

    /*UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEquipSlot EquipSlot;*/
};
