// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipWidget.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "EquipSlotWidget.h"
#include "QuickSlotWidget.h"
#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "InventoryComponent.h"
#include "ItemFactory.h"
#include "ItemBase.h"
#include "EquipableItem.h"
#include "NonEquipableItem.h"

void UEquipWidget::NativeConstruct()
{
    CreateWeaponSlots();
    CreateArmorSlots();
    CreateQuickSlots();

    //첫 슬롯 포커스
    if (Panels.Num() > 0 && Panels[0].Slots.Num() > 0) {
        CurrentPanelIndex = 0;
        CurrentSlotIndex = 0;
        Panels[0].Slots[0]->SetFocus();
    }

    CreateEquipInventorySlots(6, 6);
}

void UEquipWidget::CreateWeaponSlots()
{
    if (!EquipSlotWidgetClass || !WeaponGridPanel)
        return;

    FEquipPanelInfo Info;
    Info.Panel = WeaponGridPanel;
    Info.ColumnCount = 2;
    Info.RowCount = 2;

    for (int32 Index = 0; Index < 4; Index++) {
        auto* NewSlot = CreateWidget<UEquipSlotWidget>(this, EquipSlotWidgetClass);
        //무기 슬롯에 방어구도 장착 가능하도록 bAllowWeapon = true
        NewSlot->InitEquipSlot(Index, (Index % 2 == 0) ? EEquipSlotType::RightHand : EEquipSlotType::LeftHand, true);
        NewSlot->SetOwnerEquipWidget(this);
        Info.Slots.Add(NewSlot);
        WeaponGridPanel->AddChildToUniformGrid(NewSlot, Index / Info.ColumnCount, Index % Info.ColumnCount);
    }

    Panels.Add(Info);
}

void UEquipWidget::CreateArmorSlots()
{
    if (!EquipSlotWidgetClass || !ArmorGridPanel)
        return;

    FEquipPanelInfo Info;
    Info.Panel = ArmorGridPanel;
    Info.ColumnCount = 4;
    Info.RowCount = 1;

    TArray<EEquipSlotType> ArmorTypes = {EEquipSlotType::Head, EEquipSlotType::Chest, EEquipSlotType::Glove, EEquipSlotType::Leg};

    for (int32 Index = 0; Index < ArmorTypes.Num(); Index++) {
        auto* NewSlot = CreateWidget<UEquipSlotWidget>(this, EquipSlotWidgetClass);
        // 방어구 슬롯에도 무기 장착 허용
        NewSlot->InitEquipSlot(Index, ArmorTypes[Index], true);
        NewSlot->SetOwnerEquipWidget(this);
        Info.Slots.Add(NewSlot);
        ArmorGridPanel->AddChildToUniformGrid(NewSlot, Index / Info.ColumnCount, Index % Info.ColumnCount);
    }

    Panels.Add(Info);
}

void UEquipWidget::CreateQuickSlots()
{
    if (!QuickSlotWidgetClass || !QuickSlotGridPanel)
        return;

    FEquipPanelInfo Info;
    Info.Panel = QuickSlotGridPanel;
    Info.ColumnCount = 4;
    Info.RowCount = 2;

    for (int32 Index = 0; Index < 8; Index++) {
        auto* NewSlot = CreateWidget<UQuickSlotWidget>(this, QuickSlotWidgetClass);
        NewSlot->InitQuickSlot(Index, EEquipSlotType::QuickSlot);
        NewSlot->SetOwnerEquipWidget(this);
        Info.Slots.Add(NewSlot);
        QuickSlotGridPanel->AddChildToUniformGrid(NewSlot, Index / Info.ColumnCount, Index % Info.ColumnCount);
    }

    Panels.Add(Info);
}

void UEquipWidget::CreateEquipInventorySlots(int32 NumColumns, int32 NumRows)
{
    //슬롯 채워넣기
    if (!EquipInventoryGridPanel)
        return;

    //초기화
    EquipInventoryGridPanel->ClearChildren();
    EquipInventorySlots.Empty();

    for (int32 Row = 0; Row < NumRows; Row++) {
        for (int32 Col = 0; Col < NumColumns; Col++) {
            auto* NewSlot = CreateWidget<UInventorySlotWidget>(this, UInventorySlotWidget::StaticClass());
            //NewSlot->ClearSlot(); //빈 슬롯 초기화
            EquipInventoryGridPanel->AddChildToUniformGrid(NewSlot, Row, Col);
            EquipInventorySlots.Add(NewSlot);
        }
    }
}

void UEquipWidget::UpdateEquipInventory(EEquipSlotType SelectedSlotType)
{
    //적절한 아이템 보여주기
    if (!EquipInventoryGridPanel)
        return;

    auto* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
    if (!PlayerController)
        return;

    auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn());
    if (!PlayerCharacter)
        return;
    
    InventoryComponent = PlayerCharacter->GetInventoryComponent();
    if (!InventoryComponent)
        return;

    auto* ItemFactory = GetGameInstance()->GetSubsystem<UItemFactory>();
    if (!ItemFactory)
        return;

    bool bIsQuickSlot = (SelectedSlotType == EEquipSlotType::QuickSlot);

    //필터링
    TArray<FInventoryItemData> ShowUpItems;
    if (bIsQuickSlot)
        ShowUpItems = InventoryComponent->GetFilteredInventoryItems({EItemType::Consumable, EItemType::Quest});
    else
        ShowUpItems = InventoryComponent->GetFilteredInventoryItems({EItemType::Armor, EItemType::Weapon});

    //UI갱신
    const TArray<UWidget*> Children = EquipInventoryGridPanel->GetAllChildren();
    for (int32 Index = 0; Index < Children.Num(); Index++) {
        if (auto* SlotWidget = Cast<UInventorySlotWidget>(Children[Index])) {
            if (ShowUpItems.IsValidIndex(Index)) {
                SlotWidget->RefreshSlot(ShowUpItems, Index);
            }
            else {
                //슬롯이 비어있으면 기본 이미지 표시
                SlotWidget->ClearSlot();
            }
        }
    }
}

void UEquipWidget::OnEquipInventorySlotClicked(UInventorySlotWidget* ClickedSlot)
{

}

void UEquipWidget::MoveFocusUp()
{
    if (!Panels.IsValidIndex(CurrentPanelIndex)) 
        return;

    FEquipPanelInfo& Panel = Panels[CurrentPanelIndex];
    int32 CurrentRow = CurrentSlotIndex / Panel.ColumnCount;

    if (CurrentRow > 0) {
        //같은 패널 위쪽 이동
        CurrentSlotIndex -= Panel.ColumnCount;
    }
    else {
        //이전 패널로 이동
        int32 PrevPanelIndex = CurrentPanelIndex - 1;
        if (Panels.IsValidIndex(PrevPanelIndex)) {
            CurrentPanelIndex = PrevPanelIndex;
            CurrentSlotIndex = FMath::Min(CurrentSlotIndex, Panels[PrevPanelIndex].Slots.Num() - 1);
        }
    }

    Panels[CurrentPanelIndex].Slots[CurrentSlotIndex]->SetFocus();
}

void UEquipWidget::MoveFocusDown()
{
    if (!Panels.IsValidIndex(CurrentPanelIndex)) 
        return;

    FEquipPanelInfo& Panel = Panels[CurrentPanelIndex];
    int32 CurrentRow = CurrentSlotIndex / Panel.ColumnCount;

    if (CurrentRow < Panel.RowCount - 1 && 
        CurrentSlotIndex + Panel.ColumnCount < Panel.Slots.Num()) {
        //같은 패널 아래쪽 이동
        CurrentSlotIndex += Panel.ColumnCount;
    }
    else {
        //다음 패널로 이동
        int32 NextPanelIndex = CurrentPanelIndex + 1;
        if (Panels.IsValidIndex(NextPanelIndex)) {
            CurrentPanelIndex = NextPanelIndex;
            CurrentSlotIndex = FMath::Min(CurrentSlotIndex, Panels[NextPanelIndex].Slots.Num() - 1);
        }
    }

    Panels[CurrentPanelIndex].Slots[CurrentSlotIndex]->SetFocus();
}

void UEquipWidget::MoveFocusLeft()
{
    if (!Panels.IsValidIndex(CurrentPanelIndex)) 
        return;

    if (CurrentSlotIndex % Panels[CurrentPanelIndex].ColumnCount > 0) {
        CurrentSlotIndex -= 1;
    }
    else {
        //같은 Row의 마지막으로 이동
        CurrentSlotIndex += Panels[CurrentPanelIndex].ColumnCount - 1;
        if (CurrentSlotIndex >= Panels[CurrentPanelIndex].Slots.Num())
            CurrentSlotIndex = Panels[CurrentPanelIndex].Slots.Num() - 1;
    }

    Panels[CurrentPanelIndex].Slots[CurrentSlotIndex]->SetFocus();
}

void UEquipWidget::MoveFocusRight()
{
    if (!Panels.IsValidIndex(CurrentPanelIndex)) 
        return;

    if ((CurrentSlotIndex % Panels[CurrentPanelIndex].ColumnCount) < Panels[CurrentPanelIndex].ColumnCount - 1 &&
        CurrentSlotIndex + 1 < Panels[CurrentPanelIndex].Slots.Num()) {
        CurrentSlotIndex += 1;
    }
    else {
        //같은 Row의 처음으로 이동
        CurrentSlotIndex -= (Panels[CurrentPanelIndex].ColumnCount - 1);
        if (CurrentSlotIndex < 0) CurrentSlotIndex = 0;
    }

    Panels[CurrentPanelIndex].Slots[CurrentSlotIndex]->SetFocus();
}
