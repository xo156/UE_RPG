// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FocusableSlotWidget.h"
#include "EquipSlotType.h"
#include "ItemData.h"
#include "InventoryItemData.h"
#include "QuickSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UQuickSlotWidget : public UFocusableSlotWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void InitQuickSlot(int32 InIndex, EEquipSlotType InEquipSlotType, FInventoryItemData InQuickSlotItem);

	void RefreshSlot();

	void SetOwnerEquipWidget(class UEquipWidget* InOwner);

private:
	class UEquipWidget* OwnerEquipWidget;
	EEquipSlotType EquipSlotType;
	int32 SlotIndex;
	FInventoryItemData QuickSlotItem;
};
