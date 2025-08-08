// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "NonEquipableItemData.h"
#include "NonEquipableItem.generated.h"

UCLASS(Abstract)
class RPG_API ANonEquipableItem : public AItemBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANonEquipableItem();

	void InitNonEquipableData(const FNonEquipableItemData* InItemData);

	virtual void UseItem();

	float GetEffectValue() const {return NonEquipableItemData->EffectValue; }
	float GetEffectRadius() const {return NonEquipableItemData->EffectRadius; }
	EConsumableEffectType GetEffectType() const {return NonEquipableItemData->EffectType; }

protected:
	const FNonEquipableItemData* NonEquipableItemData;
};
