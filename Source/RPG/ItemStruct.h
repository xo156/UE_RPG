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
    //아이템 ID
    UPROPERTY(EditAnywhere)
    FName ItemID;

    //아이템 타입
    UPROPERTY(EditAnywhere)
    EItemType ItemType;

    //아이템 이름
    UPROPERTY(EditAnywhere)
    FString ItemName;

    //아이템 설명
    UPROPERTY(EditAnywhere)
    FString ItemDescription;

    //아이템 아이콘
    UPROPERTY(EditAnywhere)
    class UTexture2D* ItemIcon;

    //아이템 메쉬
    UPROPERTY(EditAnywhere)
    class UStaticMesh* ItemMesh;

    //아이템 벨류
    UPROPERTY(EditAnywhere)
    float ItemValue;

    //아이템 수량
    UPROPERTY(EditAnywhere)
    FItemNums ItemNum;

    //아이템 드롭 확률 (0.0 - 1.0 범위)
    UPROPERTY(EditAnywhere)
    float DropRate = 1.f;
};
