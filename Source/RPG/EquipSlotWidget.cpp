// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipSlotWidget.h"
#include "Components/Button.h"
#include "EquipWidget.h"

void UEquipSlotWidget::NativeConstruct()
{
    if (SlotButton)
        SlotButton->OnClicked.AddDynamic(this, &UEquipSlotWidget::OnSlotConfirmed);
}

void UEquipSlotWidget::InitEquipSlot(int32 InIndex, EEquipSlotType InEquipSlotType, bool bInAllowWeapon)
{
    SlotIndex = InIndex;
    EquipSlotType = InEquipSlotType;
    bAllowWeapon = bInAllowWeapon;
}

void UEquipSlotWidget::OnSlotConfirmed()
{
    Super::OnSlotConfirmed();

    if (OwnerEquipWidget)
        OwnerEquipWidget->UpdateEquipInventory(EquipSlotType);
}

void UEquipSlotWidget::SetOwnerEquipWidget(UEquipWidget* InOwner)
{
    if (InOwner)
        OwnerEquipWidget = InOwner;
}
