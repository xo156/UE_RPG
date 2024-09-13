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
	//������ UID
	UPROPERTY(EditAnywhere)
	int32 ItemUID;	

	//������ ���̺� ID
	UPROPERTY(EditAnywhere)
	int32 ItemTableID;

	//������ ����
	UPROPERTY(EditAnywhere)
	int32 ItemAmount;

	//������ ���� ����
	UPROPERTY(EditAnywhere)
	bool bCounterble;
};
