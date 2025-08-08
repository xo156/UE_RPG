// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryItemData.h"
#include "QuickslotData.generated.h"

USTRUCT(BlueprintType)
struct FQuickSlotData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SlotIndex = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FInventoryItemData ItemData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CooldownRemaining;

    //TODO: 나중에는 UI쪽도
};
