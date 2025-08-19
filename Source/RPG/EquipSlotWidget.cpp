// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipSlotWidget.h"
#include "Components/Button.h"
#include "EquipWidget.h"
#include "InventorySlotWidget.h"

void UEquipSlotWidget::NativeConstruct()
{
    if (SlotButton)
        SlotButton->OnClicked.AddDynamic(this, &UEquipSlotWidget::HandleClicked);
}

void UEquipSlotWidget::InitSlot(EItemType DefaultType, EArmorType InArmorCategory, bool bAllowWeaponInArmor)
{
    AllowedItemTypes.Empty();
    ArmorCategory = InArmorCategory;

    switch (DefaultType)
    {
    case EItemType::Weapon:
        AllowedItemTypes.Add(EItemType::Weapon);
        break;
    case EItemType::Armor:
        AllowedItemTypes.Add(EItemType::Armor);
        if (bAllowWeaponInArmor)
            AllowedItemTypes.Add(EItemType::Weapon); //방어구 슬롯에 무기 허용
        break;
    case EItemType::Consumable:
        AllowedItemTypes.Add(EItemType::Consumable);
        AllowedItemTypes.Add(EItemType::Quest);
        break;
    }
}

bool UEquipSlotWidget::CanEquipItem(EItemType ItemType) const
{
    return AllowedItemTypes.Contains(ItemType);
}

void UEquipSlotWidget::EquipItemFromInventory(UInventorySlotWidget* InventorySlot)
{
    if (!InventorySlot || !CanEquipItem(InventorySlot->GetCurrentInventoryItemData().ItemType))
        return;

    int32 ItemID = InventorySlot->GetCurrentInventoryItemData().ItemTableID;
    //OnItemEquipped(InventorySlot->GetCurrentInventoryItemData().ItemType, ItemID);
}
