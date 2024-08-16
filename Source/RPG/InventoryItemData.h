// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryItemData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct RPG_API FInventoryItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	//아이템 UID
	UPROPERTY(EditAnywhere)
	int32 ItemUID;

	//아이템 ID
	UPROPERTY(EditAnywhere)
	int32 ItemTableID;

	//아이템 현재 수량
	UPROPERTY(EditAnywhere)
	int32 ItemCurrentAmount;

	//아이템 최대 수량
	UPROPERTY(EditAnywhere)
	int32 ItemMaxAmount;

	//셀 수 있는 아이템인가
	UPROPERTY(EditAnywhere)
	bool bIsCountable;
};
