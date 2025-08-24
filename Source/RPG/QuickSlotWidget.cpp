// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotWidget.h"
#include "Components/Button.h"
#include "EquipWidget.h"
#include "ItemFactory.h"

void UQuickSlotWidget::NativeConstruct()
{
    if (SlotButton)
        SlotButton->OnClicked.AddDynamic(this, &UQuickSlotWidget::HandleClicked);
}

void UQuickSlotWidget::InitQuickSlot(int32 InIndex, EEquipSlotType InEquipSlotType, FInventoryItemData InQuickSlotItem)
{
    SlotIndex = InIndex;
    EquipSlotType = InEquipSlotType;
    QuickSlotItem = InQuickSlotItem;
}

void UQuickSlotWidget::RefreshSlot()
{
    auto* ItemFactory = GetGameInstance()->GetSubsystem<UItemFactory>();
    if (!ItemFactory)
        return;

    if (QuickSlotItem.ItemTableID!=0) {
        SetItemIcon(ItemFactory->FindItemData(QuickSlotItem.ItemTableID)->ItemIcon);
    }
    else {
        ClearSlot();
    }
}

void UQuickSlotWidget::SetOwnerEquipWidget(UEquipWidget* InOwner)
{
    if (InOwner)
        OwnerEquipWidget = InOwner;
}