// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDrop.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct RPG_API FItemDrop : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    //아이템 ID
    UPROPERTY(EditAnywhere)
    int32 ItemID;

    //아이템 드랍 확률 (0.f ~ 100.f)
    UPROPERTY(EditAnywhere)
    float ItemRate;

    //아이템 드랍 수량
    int32 DropItemAmount;

    //아이템 드랍 최소 수량
    UPROPERTY(EditAnywhere)
    int32 MinDropItemAmount;

    //아이템 드랍 최대 수량
    UPROPERTY(EditAnywhere)
    int32 MaxDropItemAmount;

    //셀 수 있는 아이템인지
    UPROPERTY(EditAnywhere)
    bool bIsCountable;
};
