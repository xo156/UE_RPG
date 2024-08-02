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

USTRUCT()
struct FItemNums
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
    int32 MaxStackSize;
    UPROPERTY(EditAnywhere)
    int32 CurrentStackSize;
    UPROPERTY(EditAnywhere)
    bool bIsStackable;
};

USTRUCT(BlueprintType)
struct RPG_API FItemStruct : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    //������ ID
    UPROPERTY(EditAnywhere)
    FName ItemID;

    //������ Ÿ��
    UPROPERTY(EditAnywhere)
    EItemType ItemType;

    //������ �̸�
    UPROPERTY(EditAnywhere)
    FString ItemName;

    //������ ����
    UPROPERTY(EditAnywhere)
    FString ItemDescription;

    //������ ������
    UPROPERTY(EditAnywhere)
    class UTexture2D* ItemIcon;

    //������ �޽�
    UPROPERTY(EditAnywhere)
    class UStaticMesh* ItemMesh;

    //������ ����
    UPROPERTY(EditAnywhere)
    float ItemValue;

    //������ ����
    UPROPERTY(EditAnywhere)
    FItemNums ItemNum;

    //������ ��� Ȯ�� (0.0 - 1.0 ����)
    UPROPERTY(EditAnywhere)
    float DropRate = 1.f;
};
