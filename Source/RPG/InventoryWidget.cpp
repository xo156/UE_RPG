// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventoryComponent.h"
#include "InventorySlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "ItemBase.h"
#include "ItemData.h"
#include "DropItem.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "PlayerHUD.h"
#include "DataTableGameInstance.h"
#include "ItemFactory.h"
#include "Kismet/GameplayStatics.h"
#include "EquipableItem.h"
#include "NonEquipableItem.h"
#include "FocusableUserWidget.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

    //인벤토리 메인
    InventoryName->SetText(FText::FromString(TEXT("Inventory")));

    //툴 팁
    if (ItemName) {
        ItemName->SetText(FText::FromString(""));
    }

    if (AttackPowerValue) {
        AttackPowerValue->SetText(FText::FromString(""));
    }

    if (DefensePowerValue) {
        DefensePowerValue->SetText(FText::FromString(""));
    }

    if (ItemDescription) {
        ItemDescription->SetText(FText::FromString(""));
    }

    //아이템 액션
    if (UseButton) {
        UseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnUseClicked);
    }

    if (DestroyButton) {
        DestroyButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnDestroyClicked);
    }

}

void UInventoryWidget::InitInventoryWidget(UInventoryComponent* InventoryComponent)
{
    if (!InventoryComponent)
        return;

    CreateInventorySlotWidget(InventoryComponent);
    UpdateInventorySlotWidget(InventoryComponent);

}

void UInventoryWidget::CreateInventorySlotWidget(UInventoryComponent* InventoryComponent)
{
    if (!InventoryComponent || !InventoryGridPanel || !InventorySlotWidgetClass) {
        return;
    }

    InventorySlotWidgets.Empty();
    const int32 TotalSlots = InventoryComponent->GetMaxSlotCounter();

    InventoryColumnCount = FMath::Clamp(MaxDesiredColumns, 1, TotalSlots);

    for (int32 Index = 0; Index < TotalSlots; Index++) {
        auto* NewSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
        if (NewSlot) {
            int32 Row = Index / InventoryColumnCount;
            int32 Column = Index % InventoryColumnCount;
            InventoryGridPanel->AddChildToUniformGrid(NewSlot, Row, Column);
            NewSlot->SetParentInventoryWidget(this);
            InventorySlotWidgets.Add(NewSlot);
            UE_LOG(LogTemp, Log, TEXT("Slot Created No: %d"), Index);
        }
    }
}

void UInventoryWidget::UpdateInventorySlotWidget(UInventoryComponent* InventoryComponent)
{
    if (!InventoryComponent || InventorySlotWidgets.Num() == 0) {
        return;
    }
    const TArray<FInventoryItemData>& InventoryItem = InventoryComponent->GetInventoryItems();

    for (int32 Index = 0; Index < InventorySlotWidgets.Num(); Index++) {
        if (InventorySlotWidgets[Index]) {
            if (Index < InventoryItem.Num()) {
                InventorySlotWidgets[Index]->RefreshSlot(InventoryItem[Index]);
            }
            else {
                InventorySlotWidgets[Index]->ClearSlot();
            }
        }
    }
}

void UInventoryWidget::ConfirmFocusSlot()
{
    if (!CurrentFocusedSlot) 
        return;

    auto* ItemFactory = GetGameInstance()->GetSubsystem<UItemFactory>();
    if (!ItemFactory)
        return;

    const FInventoryItemData& FocusedData = GetCurrentFocusedSlot()->GetCurrentInventoryItemData();

    bool bValidItem = FocusedData.ItemAmount > 0 || FocusedData.bRemainInInventoryWhenAmountZero;
    if (bValidItem) {
        const FItemData* ItemData = ItemFactory->FindItemData(FocusedData.ItemTableID);
        if (ItemData) {
            InitItemAction(FocusedData);
            InitTooltip(*ItemData);
        }
    }
    else {
        ClearToolTip();
        ClearItemAction();
    }
}

void UInventoryWidget::InitTooltip(const FItemData& InItemData)
{
    if (ItemName) {
        ItemName->SetText(FText::FromString(InItemData.ItemName.ToString()));
    }

    //TODO: FItemData에 없는 정보를 캐스팅해서 가져오도록 할 것
    //TODO: 그런데 이거 누를때마다 캐스팅이 맞는건가 싶긴 해
    if (AttackPowerValue && DefensePowerValue) {
        switch (InItemData.ItemType) {
        case EItemType::Consumable:
            //AttackPowerValue->SetText(FText::FromString("회복량: {0}"), FText::AsNumber(InItemData.ItemAttackValue));
            break;
        case EItemType::Weapon:
        case EItemType::Armor:
        {
            //AttackPowerValue->SetText(FText::FromString("공격력: {0}"), FText::AsNumber(InItemData.ItemAttackValue));
            //DefensePowerValue->SetText(FText::FromString("방어력: {0}"), FText::AsNumber(InItemData.ItemAttackValue));
        }
        case EItemType::Quest:
            AttackPowerValue->SetText(FText::FromString(""));
            DefensePowerValue->SetText(FText::FromString(""));
            break;
        default:
            AttackPowerValue->SetText(FText::FromString(""));
            DefensePowerValue->SetText(FText::FromString(""));
            break;
        }
    }

    if (ItemDescription) {
        ItemDescription->SetText(FText::FromString(InItemData.ItemDescription));
    }
}

void UInventoryWidget::ClearToolTip()
{
    if (ItemName) {
        ItemName->SetText(FText::FromString(""));
    }

    if (AttackPowerValue) {
        AttackPowerValue->SetText(FText::FromString(""));
    }

    if (DefensePowerValue) {
        DefensePowerValue->SetText(FText::FromString(""));
    }

    if (ItemDescription) {
        ItemDescription->SetText(FText::FromString(""));
    }
}

void UInventoryWidget::InitItemAction(const FInventoryItemData& InItemData)
{
    InventoryItemData = InItemData;
}

void UInventoryWidget::ClearItemAction()
{
    InventoryItemData = FInventoryItemData();
}

void UInventoryWidget::OnUseClicked()
{
    if (InventoryItemData.ItemAmount <= 0)
        return;

    auto* PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!PlayerController)
        return;

    auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn());
    if (!PlayerCharacter)
        return;

    auto* ItemFactory = GetGameInstance()->GetSubsystem<UItemFactory>();
    if (!ItemFactory)
        return;

    auto* InventoryComponent = PlayerCharacter->GetInventoryComponent();
    if (!InventoryComponent)
        return;

    auto* SpawnedItem = ItemFactory->SpawnItemFromTableID(GetWorld(), InventoryItemData.ItemTableID);
    if (SpawnedItem) {
        switch (SpawnedItem->GetItemType())
        {
        case EItemType::Consumable:
        case EItemType::Quest:
        {
            auto* NonEquipableItem = Cast<ANonEquipableItem>(SpawnedItem);
            if (!NonEquipableItem)
                return;

            NonEquipableItem->UseItem();
        }
        default:
            break;
        }
        InventoryComponent->RemoveItem(InventoryItemData.ItemTableID, 1);
        UpdateInventorySlotWidget(InventoryComponent);
    }
}

void UInventoryWidget::OnDestroyClicked()
{
    if (InventoryItemData.ItemAmount <= 0)
        return;

    auto* PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!PlayerController)
        return;

    auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn());
    if (!PlayerCharacter)
        return;

    auto* InventoryComponent = PlayerCharacter->GetInventoryComponent();
    if (!InventoryComponent)
        return;

    InventoryComponent->RemoveItem(InventoryItemData.ItemTableID, 1);

    UpdateInventorySlotWidget(InventoryComponent);
}

void UInventoryWidget::MoveFocusUp()
{
    if (InventorySlotWidgets.Num() == 0 || InventoryColumnCount <= 0) return;

    if (!CurrentFocusedSlot) {
        RequestFocus(InventorySlotWidgets[0]);
        return;
    }

    UInventorySlotWidget* Focused = GetCurrentFocusedSlot();
    if (!Focused) return;

    const int32 CurrentIndex = InventorySlotWidgets.IndexOfByKey(Focused);
    if (CurrentIndex == INDEX_NONE) return;

    const int32 NewIndex = CurrentIndex - InventoryColumnCount;
    if (NewIndex >= 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Inventory MoveFocusUp -> %d"), NewIndex);
        RequestFocus(InventorySlotWidgets[NewIndex]);
    }
}

void UInventoryWidget::MoveFocusDown()
{
    if (InventorySlotWidgets.Num() == 0 || InventoryColumnCount <= 0) return;

    if (!CurrentFocusedSlot) {
        RequestFocus(InventorySlotWidgets[0]);
        return;
    }

    UInventorySlotWidget* Focused = GetCurrentFocusedSlot();
    if (!Focused) return;

    const int32 CurrentIndex = InventorySlotWidgets.IndexOfByKey(Focused);
    if (CurrentIndex == INDEX_NONE) return;

    const int32 NewIndex = CurrentIndex + InventoryColumnCount;
    if (NewIndex < InventorySlotWidgets.Num())
    {
        UE_LOG(LogTemp, Log, TEXT("Inventory MoveFocusDown -> %d"), NewIndex);
        RequestFocus(InventorySlotWidgets[NewIndex]);
    }
}

void UInventoryWidget::MoveFocusLeft()
{
    if (InventorySlotWidgets.Num() == 0 || InventoryColumnCount <= 0) return;

    if (!CurrentFocusedSlot) {
        RequestFocus(InventorySlotWidgets[0]);
        return;
    }

    UInventorySlotWidget* Focused = GetCurrentFocusedSlot();
    if (!Focused) return;

    const int32 CurrentIndex = InventorySlotWidgets.IndexOfByKey(Focused);
    if (CurrentIndex == INDEX_NONE) return;

    const int32 Col = CurrentIndex % InventoryColumnCount;
    if (Col > 0)
    {
        const int32 NewIndex = CurrentIndex - 1;
        UE_LOG(LogTemp, Log, TEXT("Inventory MoveFocusLeft -> %d"), NewIndex);
        RequestFocus(InventorySlotWidgets[NewIndex]);
    }
}

void UInventoryWidget::MoveFocusRight()
{
    if (InventorySlotWidgets.Num() == 0 || InventoryColumnCount <= 0) return;

    if (!CurrentFocusedSlot) {
        RequestFocus(InventorySlotWidgets[0]);
        return;
    }

    UInventorySlotWidget* Focused = GetCurrentFocusedSlot();
    if (!Focused) return;

    const int32 CurrentIndex = InventorySlotWidgets.IndexOfByKey(Focused);
    if (CurrentIndex == INDEX_NONE) return;

    const int32 Col = CurrentIndex % InventoryColumnCount;
    // 마지막 열이 아니고, 실제 슬롯이 존재할 때만 이동
    if (Col < InventoryColumnCount - 1 && (CurrentIndex + 1) < InventorySlotWidgets.Num())
    {
        const int32 NewIndex = CurrentIndex + 1;
        UE_LOG(LogTemp, Log, TEXT("Inventory MoveFocusRight -> %d"), NewIndex);
        RequestFocus(InventorySlotWidgets[NewIndex]);
    }
}