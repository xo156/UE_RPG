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

    //ù ���� ��Ŀ��
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
        //���� ���Կ� ���� ���� �����ϵ��� bAllowWeapon = true
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
        // �� ���Կ��� ���� ���� ���
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
    //���� ä���ֱ�
    if (!EquipInventoryGridPanel)
        return;

    //�ʱ�ȭ
    EquipInventoryGridPanel->ClearChildren();
    EquipInventorySlots.Empty();

    for (int32 Row = 0; Row < NumRows; Row++) {
        for (int32 Col = 0; Col < NumColumns; Col++) {
            auto* NewSlot = CreateWidget<UInventorySlotWidget>(this, UInventorySlotWidget::StaticClass());
            //NewSlot->ClearSlot(); //�� ���� �ʱ�ȭ
            EquipInventoryGridPanel->AddChildToUniformGrid(NewSlot, Row, Col);
            EquipInventorySlots.Add(NewSlot);
        }
    }
}

void UEquipWidget::UpdateEquipInventory(EEquipSlotType SelectedSlotType)
{
    //������ ������ �����ֱ�
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

    //���͸�
    TArray<FInventoryItemData> ShowUpItems;
    if (bIsQuickSlot)
        ShowUpItems = InventoryComponent->GetFilteredInventoryItems({EItemType::Consumable, EItemType::Quest});
    else
        ShowUpItems = InventoryComponent->GetFilteredInventoryItems({EItemType::Armor, EItemType::Weapon});

    //UI����
    const TArray<UWidget*> Children = EquipInventoryGridPanel->GetAllChildren();
    for (int32 Index = 0; Index < Children.Num(); Index++) {
        if (auto* SlotWidget = Cast<UInventorySlotWidget>(Children[Index])) {
            if (ShowUpItems.IsValidIndex(Index)) {
                SlotWidget->RefreshSlot(ShowUpItems, Index);
            }
            else {
                //������ ��������� �⺻ �̹��� ǥ��
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
        //���� �г� ���� �̵�
        CurrentSlotIndex -= Panel.ColumnCount;
    }
    else {
        //���� �гη� �̵�
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
        //���� �г� �Ʒ��� �̵�
        CurrentSlotIndex += Panel.ColumnCount;
    }
    else {
        //���� �гη� �̵�
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
        //���� Row�� ���������� �̵�
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
        //���� Row�� ó������ �̵�
        CurrentSlotIndex -= (Panels[CurrentPanelIndex].ColumnCount - 1);
        if (CurrentSlotIndex < 0) CurrentSlotIndex = 0;
    }

    Panels[CurrentPanelIndex].Slots[CurrentSlotIndex]->SetFocus();
}
