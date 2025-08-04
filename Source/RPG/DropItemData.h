// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DropItemData.generated.h"

USTRUCT(BlueprintType)
struct RPG_API FDropItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
    //������ ID
    UPROPERTY(EditAnywhere)
    int32 ItemTableID;

    //��� ����
    UPROPERTY(EditAnywhere)
    int32 Amount;
};
