// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipSlotWidget.h"
#include "Components/Button.h"
#include "EquipWidget.h"
#include "InventorySlotWidget.h"
#include "ItemFactory.h"
#include "ItemBase.h"
#include "EquipableItem.h"
#include "EquipComponent.h"

void UEquipSlotWidget::NativeConstruct()
{
    if (SlotButton)
        SlotButton->OnClicked.AddDynamic(this, &UEquipSlotWidget::HandleClicked);
}

void UEquipSlotWidget::InitSlot(EItemType DefaultType, EArmorType InArmorCategory, EEquipSlotType InSlotType, bool bAllowWeaponInArmor)
{
    AllowedItemTypes.Empty();
    ArmorCategory = InArmorCategory;
    EquipSlotType = InSlotType;

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

void UEquipSlotWidget::RefreshSlot()
{
    auto* ItemFactory = GetGameInstance()->GetSubsystem<UItemFactory>();
    if (!ItemFactory)
        return;

    if (EquippedItemID != 0) {
        SetItemIcon(ItemFactory->FindItemData(EquippedItemID)->ItemIcon);
    }
    else {
        ClearSlot();
    }
}

bool UEquipSlotWidget::CanEquipItem(EItemType ItemType) const
{
    return AllowedItemTypes.Contains(ItemType);
}

void UEquipSlotWidget::EquipItemFromInventory(UInventorySlotWidget* InventorySlot)
{
    if (!InventorySlot || !EquipComponent)
        return;

    if(!CanEquipItem(InventorySlot->GetCurrentInventoryItemData().ItemType))
       return;

    EquipComponent->EquipItem(InventorySlot->GetCurrentInventoryItemData(), EquipSlotType);
    EquippedItemID = InventorySlot->GetCurrentInventoryItemData().ItemTableID;
}
