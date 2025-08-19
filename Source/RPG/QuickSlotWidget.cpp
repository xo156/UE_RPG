// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotWidget.h"
#include "Components/Button.h"
#include "EquipWidget.h"

void UQuickSlotWidget::NativeConstruct()
{
    if (SlotButton)
        SlotButton->OnClicked.AddDynamic(this, &UQuickSlotWidget::HandleClicked);
}

void UQuickSlotWidget::InitQuickSlot(int32 InIndex, EEquipSlotType InEquipSlotType)
{
    SlotIndex = InIndex;
    EquipSlotType = InEquipSlotType;
}

void UQuickSlotWidget::SetOwnerEquipWidget(UEquipWidget* InOwner)
{
    if (InOwner)
        OwnerEquipWidget = InOwner;
}