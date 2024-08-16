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
    //������ ID
    UPROPERTY(EditAnywhere)
    int32 ItemID;

    //������ ��� Ȯ�� (0.f ~ 100.f)
    UPROPERTY(EditAnywhere)
    float ItemRate;

    //������ ��� ����
    int32 DropItemAmount;

    //������ ��� �ּ� ����
    UPROPERTY(EditAnywhere)
    int32 MinDropItemAmount;

    //������ ��� �ִ� ����
    UPROPERTY(EditAnywhere)
    int32 MaxDropItemAmount;

    //�� �� �ִ� ����������
    UPROPERTY(EditAnywhere)
    bool bIsCountable;
};
