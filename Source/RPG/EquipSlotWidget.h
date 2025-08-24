// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FocusableSlotWidget.h"
#include "EquipSlotType.h"
#include "ItemData.h"
#include "EquipSlotWidget.generated.h"


UCLASS()
class RPG_API UEquipSlotWidget : public UFocusableSlotWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void InitSlot(EItemType DefaultType, EArmorType InArmorCategory, EEquipSlotType InSlotType, bool bAllowWeaponInArmor);
	EEquipSlotType GetEquipSlotType() const { return EquipSlotType; }

	void RefreshSlot();

	int32 GetEquippedItemID() { return EquippedItemID; }

	bool CanEquipItem(EItemType ItemType) const;

	void EquipItemFromInventory(class UInventorySlotWidget* InventorySlot); //아이템 장착

private:
	class UEquipComponent* EquipComponent;
	EEquipSlotType EquipSlotType;
	TArray<EItemType> AllowedItemTypes; //허용된 아이템 타입
	EArmorType ArmorCategory; //방어구 종류
	int32 EquippedItemID; //장착된 아이템 ID
};
