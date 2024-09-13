// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

UENUM()
enum class EItemType : uint8
{
    Weapon UMETA(DisplayName = "Weapon"),
    Consumable UMETA(DisplayName = "Consumable")
};

USTRUCT(BlueprintType)
struct RPG_API FItemData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    //������ �̸�
    UPROPERTY(EditAnywhere)
    FText ItemName;

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
    int32 ItemAttackValue;

    //������ ȸ����
    UPROPERTY(EditAnywhere)
    float ItemRecoverValue;

    //������ Ŭ����
    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<class AItemBase>> ItemClass;

};
