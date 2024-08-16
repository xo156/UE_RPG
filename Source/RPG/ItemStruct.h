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
    //아이템 이름
    UPROPERTY(EditAnywhere)
    FString ItemName;

    //아이템 아이콘
    UPROPERTY(EditAnywhere)
    class UTexture2D* ItemIcon;

    //아이템 타입
    UPROPERTY(EditAnywhere)
    EItemType ItemType;

    //아이템 설명
    UPROPERTY(EditAnywhere)
    FString ItemDescription;

    //아이템 공격력
    UPROPERTY(EditAnywhere)
    int32 ItemAttackPoint;

    //아이템 클래스
    UPROPERTY(EditAnywhere)
    TSubclassOf<class AItemBase> ItemClass;
};
