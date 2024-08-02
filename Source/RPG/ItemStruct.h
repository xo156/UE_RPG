// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemStruct.generated.h"

UENUM()
enum class EItemType : uint8
{
    Weapon UMETA(DisplayName = "Weapon"),
    Consumable UMETA(DisplayName = "Consumable")
};

USTRUCT(BlueprintType)
struct RPG_API FItemStruct : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    //�������� Ÿ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType;

    //�������� �̸�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName;

    //������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemDescription;

    //������ ������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    class UTexture2D* ItemIcon;

    //������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 MaxStackSize;
    int32 CurrentStackSize;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bIsStackable;

    //������ ��� Ȯ�� (0.0 - 1.0 ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float DropRate = 1.f;
};
