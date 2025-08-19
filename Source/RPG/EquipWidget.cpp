// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipWidget.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "FocusableSlotWidget.h"
#include "EquipSlotWidget.h"
#include "QuickSlotWidget.h"
#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "InventoryComponent.h"
#include "EquipComponent.h"
#include "ItemFactory.h"
#include "ItemBase.h"
#include "EquipableItem.h"
#include "NonEquipableItem.h"

void UEquipWidget::NativeConstruct()
{
    CurrentFocusedSlot = nullptr;
    CurrentClickedEquipSlot = nullptr;
    CreateAllSlots();

}

void UEquipWidget::CreateAllSlots()
{
    int32 CurrentRow = 0;

    //무기 2x2
    for (int32 Row = 0; Row < 2; Row++) {
        for (int32 Col = 0; Col < 2; Col++) {
            auto* NewSlot = CreateWidget<UEquipSlotWidget>(this, EquipSlotWidgetClass);
            NewSlot->InitSlot(EItemType::Weapon);
            SlotGridPanel->AddChildToUniformGrid(NewSlot, CurrentRow + Row, Col);
            RegisterSlot(NewSlot, CurrentRow + Row, Col);
        }
    }
    CurrentRow += 2;

    //방어구 1x4
    TArray<EArmorType> ArmorTypes = {EArmorType::Head, EArmorType::Chest, EArmorType::Glove, EArmorType::Leg};
    for (int32 Col = 0; Col < ArmorTypes.Num(); Col++) {
        auto* NewSlot = CreateWidget<UEquipSlotWidget>(this, EquipSlotWidgetClass);
        NewSlot->InitSlot(EItemType::Armor, ArmorTypes[Col], true); //방어구 슬롯에 무기 허용
        SlotGridPanel->AddChildToUniformGrid(NewSlot, CurrentRow, Col);
        RegisterSlot(NewSlot, CurrentRow, Col);
    }
    CurrentRow += 1;

    //퀵 슬롯 2x4
    for (int32 Row = 0; Row < 2; Row++) {
        for (int32 Col = 0; Col < 4; Col++) {
            auto* NewSlot = CreateWidget<UInventorySlotWidget>(this, QuickSlotWidgetClass);
            //Slot->SetItem(EItemType::Consumable, FName(*FString::Printf(TEXT("Quick_%d_%d"), Row, Col)));
            SlotGridPanel->AddChildToUniformGrid(NewSlot, CurrentRow + Row, Col);
            RegisterSlot(NewSlot, CurrentRow + Row, Col);
        }
    }
}

void UEquipWidget::RegisterSlot(UFocusableSlotWidget* FocusableSlot, int32 Row, int32 Col)
{
    if (!FocusableSlot)
        return;

    AllSlots.Add(FocusableSlot);
    SlotPositions.Add(FocusableSlot, FIntPoint(Row, Col));
    PositionToSlot.Add(FIntPoint(Row, Col), FocusableSlot);

    //클릭 이벤트 연결
    FocusableSlot->OnSlotClicked.AddDynamic(this, &UEquipWidget::HandleEquipSlotClicked);
    FocusableSlot->SetOwnerWidget(this);

    //첫 슬롯 포커스 지정
    if (!CurrentFocusedSlot)
        UpdateFocus(FocusableSlot);
}

void UEquipWidget::UpdateFocus(UFocusableSlotWidget* NewFocusedSlot)
{
    if (!NewFocusedSlot) 
        return;
    CurrentFocusedSlot = NewFocusedSlot;

    CurrentFocusedSlot->SetIsFocused(true);
}

UFocusableSlotWidget* UEquipWidget::GetSlotAtOffset(int32 dRow, int32 dCol)
{
    if (!CurrentFocusedSlot) 
        return nullptr;
    FIntPoint CurrentPos = SlotPositions[CurrentFocusedSlot];
    FIntPoint TargetPos = CurrentPos + FIntPoint(dRow, dCol);
    return PositionToSlot.Contains(TargetPos) ? PositionToSlot[TargetPos] : nullptr;
}


void UEquipWidget::HandleEquipSlotClicked(UFocusableSlotWidget* ClickedSlot)
{
    //EquipSlot 클릭 처리
    if (!ClickedSlot)
        return;
    if (auto* EquipSlot = Cast<UEquipSlotWidget>(ClickedSlot)) {
        UE_LOG(LogTemp, Log, TEXT("Equip Slot Clicked: %s"), *EquipSlot->GetName());
        // 장비 처리
    }
    else if (auto* InventorySlot = Cast<UInventorySlotWidget>(ClickedSlot)) {
        UE_LOG(LogTemp, Log, TEXT("Inventory Slot Clicked: %s"), *InventorySlot->GetName());
        // 인벤토리 처리
        //InventoryWidget->OpenInventoryForSlot(EquipSlot);
    }
}

void UEquipWidget::HandleInventorySlotClicked(UInventorySlotWidget* InventorySlot)
{
    //InventorySlot 클릭 처리 → 장착
    if (!CurrentClickedEquipSlot || !InventorySlot) 
        return;
    CurrentClickedEquipSlot->EquipItemFromInventory(InventorySlot);
    //InventoryWidget->CloseInventory();
    CurrentClickedEquipSlot = nullptr;
}

void UEquipWidget::MoveFocusUp() 
{ 
    if (auto NewFocusSlot = GetSlotAtOffset(-1, 0)) RequestFocus(NewFocusSlot);
}

void UEquipWidget::MoveFocusDown()
{ 
    if (auto NewFocusSlot = GetSlotAtOffset(1, 0)) RequestFocus(NewFocusSlot);
}

void UEquipWidget::MoveFocusLeft()
{ 
    if (auto NewFocusSlot = GetSlotAtOffset(0, -1)) RequestFocus(NewFocusSlot);
}

void UEquipWidget::MoveFocusRight()
{ 
    if (auto NewFocusSlot = GetSlotAtOffset(0, 1)) RequestFocus(NewFocusSlot);
}