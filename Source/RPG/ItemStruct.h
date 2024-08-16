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
    //������ �̸�
    UPROPERTY(EditAnywhere)
    FString ItemName;

    //������ ������
    UPROPERTY(EditAnywhere)
    class UTexture2D* ItemIcon;

    //������ Ÿ��
    UPROPERTY(EditAnywhere)
    EItemType ItemType;

    //������ ����
    UPROPERTY(EditAnywhere)
    FString ItemDescription;

    //������ ���ݷ�
    UPROPERTY(EditAnywhere)
    int32 ItemAttackPoint;

    //������ Ŭ����
    UPROPERTY(EditAnywhere)
    TSubclassOf<class AItemBase> ItemClass;
};
