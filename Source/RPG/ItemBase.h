// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "ItemBase.generated.h"

UCLASS(Abstract)
class RPG_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:
	virtual void InitItemData(const FItemData* InItemData);

	int32 GetItemTableID() const { return ItemData->ItemTableID; }
	EItemType GetItemType() const { return ItemData->ItemType; }
	class UTexture2D* GetItemIcon() const { return ItemData->ItemIcon; }

protected:
	const FItemData* ItemData;
};
