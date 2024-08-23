// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DropRate.generated.h"

USTRUCT(BlueprintType)
struct RPG_API FDropRate : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    //아이템 확률
    UPROPERTY(EditAnywhere)
    float Rate;

    //아이템 ID
    UPROPERTY(EditAnywhere)
    int32 ItemID;

    //아이템 누적 여부
    UPROPERTY(EditAnywhere)
    bool bCounterble;

    //최소 드랍 갯수
    UPROPERTY(EditAnywhere)
    int32 MinAmount;

    //최대 드랍 갯수
    UPROPERTY(EditAnywhere)
    int32 MaxAmount;

};

