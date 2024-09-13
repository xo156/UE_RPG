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

	//아이템 테이블 ID
	UPROPERTY(EditAnywhere)
	int32 ItemTableID;

	//아이템 수량
	UPROPERTY(EditAnywhere)
	int32 ItemAmount;

	//아이템 누적 여부
	UPROPERTY(EditAnywhere)
	bool bCounterble;
};
