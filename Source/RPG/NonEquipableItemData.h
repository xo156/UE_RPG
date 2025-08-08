// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "NonEquipableItemData.generated.h"

UENUM(BlueprintType)
enum class EConsumableEffectType : uint8
{
    Heal UMETA(DisplayName = "Heal"),
    Bomb UMETA(DisplayName = "Bomb"),
    Buff UMETA(DisplayName = "Buff"),
    DeBuff UMETA(DisplayName = "DeBuff"),
    Quest UMETA(DisplayName = "Quest"),
};

USTRUCT(BlueprintType)
struct RPG_API FNonEquipableItemData : public FItemData
{
	GENERATED_USTRUCT_BODY()

    //소모품 효과 타입
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EConsumableEffectType EffectType;

    //데미지, 힐, 버프 수치등
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EffectValue = 0.f;

    //반경
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EffectRadius = 0.f;
};
