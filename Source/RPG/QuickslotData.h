// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ConsumableItem.h"
#include "QuickslotData.generated.h"

USTRUCT(BlueprintType)
struct FQuickslotData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 SlotIndex = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ItemTableID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    AConsumableItem* QuickSlotedItem;

    //TODO: 나중에는 UI쪽도
};
