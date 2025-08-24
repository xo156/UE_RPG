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

    // 기본 포커스 맵은 장비 슬롯
    ActiveSlotPositions = &EquipSlotPositions;
    ActivePositionToSlot = &EquipPositionToSlot;

    CreateAllSlots();
}

void UEquipWidget::CreateAllSlots()
{
    int32 CurrentRow = 0;

    //무기 2x2
    for (int32 Row = 0; Row < 2; Row++) {
        for (int32 Col = 0; Col < 2; Col++) {
            auto* NewSlot = CreateWidget<UEquipSlotWidget>(this, EquipSlotWidgetClass);
            NewSlot->InitSlot(EItemType::Weapon, EArmorType::None, Row%2 ? EEquipSlotType::LeftHand : EEquipSlotType::RightHand, false);
            SlotGridPanel->AddChildToUniformGrid(NewSlot, CurrentRow + Row, Col);
            RegisterSlot(false, NewSlot, CurrentRow + Row, Col);
        }
    }
    CurrentRow += 2;

    //방어구 1x4
    TArray<EArmorType> ArmorTypes = {EArmorType::Head, EArmorType::Body, EArmorType::Arm, EArmorType::Leg};
    TArray<EEquipSlotType> ArmorSlotTypes = {EEquipSlotType::Head, EEquipSlotType::Body, EEquipSlotType::Arm, EEquipSlotType::Leg};
    for (int32 Col = 0; Col < ArmorTypes.Num(); Col++) {
        auto* NewSlot = CreateWidget<UEquipSlotWidget>(this, EquipSlotWidgetClass);
        NewSlot->InitSlot(EItemType::Armor, ArmorTypes[Col], ArmorSlotTypes[Col], true); //방어구 슬롯에 무기 허용
        SlotGridPanel->AddChildToUniformGrid(NewSlot, CurrentRow, Col);
        RegisterSlot(false, NewSlot, CurrentRow, Col);
    }
    CurrentRow += 1;

    //퀵 슬롯 2x4
    for (int32 Row = 0; Row < 2; Row++) {
        for (int32 Col = 0; Col < 4; Col++) {
            auto* NewSlot = CreateWidget<UQuickSlotWidget>(this, QuickSlotWidgetClass);
            //Slot->SetItem(EItemType::Consumable, FName(*FString::Printf(TEXT("Quick_%d_%d"), Row, Col)));
            SlotGridPanel->AddChildToUniformGrid(NewSlot, CurrentRow + Row, Col);
            RegisterSlot(false, NewSlot, CurrentRow + Row, Col);
        }
    }
    CurrentRow += 2;

    //전용 인벤토리 6x6
    for (int32 Row = 0; Row < 6; Row++) {
        for (int32 Col = 0; Col < 6; Col++) {
            auto* NewSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
            EquipInventorySlotGridPanel->AddChildToUniformGrid(NewSlot, CurrentRow + Row, Col);
            RegisterSlot(true, NewSlot, CurrentRow + Row, Col);
        }
    }
}

void UEquipWidget::RegisterSlot(bool bIsEquipInventory, UFocusableSlotWidget* FocusableSlot, int32 Row, int32 Col)
{
    if (!FocusableSlot)
        return;

    //전용 인벤토리 슬롯은 따로
    if (bIsEquipInventory) {
        if (auto* InventorySlot = Cast<UInventorySlotWidget>(FocusableSlot)) {
            EquipInventorySlots.Add(InventorySlot);
            EquipInventorySlotPositions.Add(FocusableSlot, FIntPoint(Row, Col));
            EquipInventoryPositionToSlot.Add(FIntPoint(Row, Col), FocusableSlot);

            //클릭 이벤트 연결
            FocusableSlot->OnSlotClicked.AddDynamic(this, &UEquipWidget::HandleEquipInventorySlotClicked);
            FocusableSlot->SetOwnerWidget(this);
        }
    }
    else {
        EquipmentSlots.Add(FocusableSlot);
        EquipSlotPositions.Add(FocusableSlot, FIntPoint(Row, Col));
        EquipPositionToSlot.Add(FIntPoint(Row, Col), FocusableSlot);

        //클릭 이벤트 연결
        FocusableSlot->OnSlotClicked.AddDynamic(this, &UEquipWidget::HandleEquipSlotClicked);
        FocusableSlot->SetOwnerWidget(this);
    }

    //첫 슬롯 포커스 지정
    if (!CurrentFocusedSlot)
        RequestFocus(FocusableSlot);
}

UFocusableSlotWidget* UEquipWidget::GetSlotAtOffset(int32 dRow, int32 dCol)
{
    if (!CurrentFocusedSlot || !ActiveSlotPositions || !ActivePositionToSlot)
        return nullptr;

    FIntPoint CurrentPos = (*ActiveSlotPositions)[CurrentFocusedSlot];
    FIntPoint TargetPos = CurrentPos + FIntPoint(dRow, dCol);
    return EquipPositionToSlot.Contains(TargetPos) ? (*ActivePositionToSlot)[TargetPos] : nullptr;
}

void UEquipWidget::UpdateAllPanels()
{
    if (!EquipComponent || !QuickSlotComponent)
        return;

    UpdateEquipPanel();
    UpdateQuickPanel();
}

void UEquipWidget::UpdateEquipPanel()
{
    if (!EquipComponent)
        return;

    for (auto* FocusableSlot : EquipmentSlots) {
        if (auto* EquipSlot = Cast<UEquipSlotWidget>(FocusableSlot)) {
            EquipSlot->RefreshSlot();
        }
    }
}

void UEquipWidget::UpdateQuickPanel()
{
    if (!QuickSlotComponent)
        return;

    for (auto* FocusableSlot : EquipmentSlots) {
        if (auto* QuickSlot = Cast<UQuickSlotWidget>(FocusableSlot)) {
            QuickSlot->RefreshSlot();
        }
    }
}

void UEquipWidget::UpdateInventoryPanel(UInventoryComponent* InventoryComponent, EEquipSlotType TargetSlotType)
{
    if (!InventoryComponent)
        return;

    // 슬롯 타입별 필터 정의
    TArray<EItemType> Filters;
    switch (TargetSlotType)
    {
    case EEquipSlotType::RightHand:
    case EEquipSlotType::LeftHand:
    {
        Filters = {EItemType::Weapon};
        break;
    }
    case EEquipSlotType::Head:
    case EEquipSlotType::Body:
    case EEquipSlotType::Arm:
    case EEquipSlotType::Leg:
    {
        Filters = {EItemType::Armor, EItemType::Weapon};
        break;
    }
    case EEquipSlotType::QuickSlot:
        Filters = {EItemType::Consumable, EItemType::Quest};
        break;

    default:
        break;
    }

    TArray<FInventoryItemData> FilteredItems = InventoryComponent->GetFilteredInventoryItems(Filters);

    int32 ItemIndex = 0;
    for (UInventorySlotWidget* EquipInventorySlot : EquipInventorySlots) {
        if (!EquipInventorySlot)
            continue;

        if (ItemIndex < FilteredItems.Num()) {
            EquipInventorySlot->RefreshSlot(FilteredItems[ItemIndex]);
            ItemIndex++;
        }
        else {
            EquipInventorySlot->ClearSlot();
        }
    }
}

void UEquipWidget::OpenEquipInventory()
{
    if (EquipInventorySlotGridPanel) {
        EquipInventorySlotGridPanel->SetVisibility(ESlateVisibility::Visible);
    }

    //포커스 맵 전환
    ActiveSlotPositions = &EquipInventorySlotPositions;
    ActivePositionToSlot = &EquipInventoryPositionToSlot;

    RequestFocus(EquipInventorySlots[0]);

    if (EquipComponent)
        EquipComponent->SetIsOpen(true);
}

void UEquipWidget::CloseEquipInventory()
{
    if (EquipInventorySlotGridPanel) {
        EquipInventorySlotGridPanel->SetVisibility(ESlateVisibility::Hidden);
    }

    // 포커스 맵 전환
    ActiveSlotPositions = &EquipSlotPositions;
    ActivePositionToSlot = &EquipPositionToSlot;

    if (EquipComponent)
        EquipComponent->SetIsOpen(false);
}

void UEquipWidget::HandleEquipSlotClicked(UFocusableSlotWidget* ClickedSlot)
{
    //EquipSlot 클릭 처리
    if (!ClickedSlot)
        return;

    if (auto* EquipSlot = Cast<UEquipSlotWidget>(ClickedSlot)) {
        CurrentClickedEquipSlot = EquipSlot;
        if (auto* PlayerController = Cast<AMyPlayerController>(GetOwningPlayer())) {
            if (auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn())) {
                if (auto* InventoryComponent = PlayerCharacter->GetInventoryComponent()) {
                    UpdateInventoryPanel(InventoryComponent, CurrentClickedEquipSlot->GetEquipSlotType());
                }
            }
        }
        OpenEquipInventory();
    }
}

void UEquipWidget::HandleEquipInventorySlotClicked(UFocusableSlotWidget* ClickedSlot)
{
    if (!ClickedSlot)
        return;

    if (auto* InventorySlot = Cast<UInventorySlotWidget>(ClickedSlot)) {
        if (!CurrentClickedEquipSlot)
            return;

        CurrentClickedEquipSlot->EquipItemFromInventory(InventorySlot);
        CloseEquipInventory();
        CurrentClickedEquipSlot = nullptr;
    }
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