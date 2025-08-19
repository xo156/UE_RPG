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
    //������ �̸�
    UPROPERTY(EditAnywhere)
    FText ItemName;

    //������ ID
    UPROPERTY(EditAnywhere)
    int32 ItemTableID;

    //������ ������
    UPROPERTY(EditAnywhere)
    class UTexture2D* ItemIcon;

    //������ Ÿ��
    UPROPERTY(EditAnywhere)
    EItemType ItemType;

    //������ �ִ� ���� ����
    UPROPERTY(EditAnywhere)
    int32 MaxStackCount = 99;

    //������ ����
    UPROPERTY(EditAnywhere)
    FString ItemDescription;

    //������ Ŭ���� ���
    UPROPERTY(EditAnywhere)
    FSoftClassPath ItemClassPath;
};
