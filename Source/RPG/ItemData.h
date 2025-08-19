// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

UENUM()
enum class EItemType : uint8
{
    Weapon UMETA(DisplayName = "Weapon"),
    Armor UMETA(DisplayName = "Armor"),
    Consumable UMETA(DisplayName = "Consumable"),
    Quest UMETA(DisplayName = "Quest")
};

UENUM(BlueprintType)
enum class EArmorType : uint8
{
    Head UMETA(DisplayName = "Head"),
    Chest UMETA (DisplayName = "Chest"),
    Glove UMETA(DisplayName = "Glove"),
    Leg UMETA(DisplayName = "Leg")
};

USTRUCT(BlueprintType)
struct RPG_API FItemData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    //아이템 이름
    UPROPERTY(EditAnywhere)
    FText ItemName;

    //아이템 ID
    UPROPERTY(EditAnywhere)
    int32 ItemTableID;

    //아이템 아이콘
    UPROPERTY(EditAnywhere)
    class UTexture2D* ItemIcon;

    //아이템 타입
    UPROPERTY(EditAnywhere)
    EItemType ItemType;

    //아이템 최대 누적 수량
    UPROPERTY(EditAnywhere)
    int32 MaxStackCount = 99;

    //아이템 설명
    UPROPERTY(EditAnywhere)
    FString ItemDescription;

    //아이템 클래스 경로
    UPROPERTY(EditAnywhere)
    FSoftClassPath ItemClassPath;
};
