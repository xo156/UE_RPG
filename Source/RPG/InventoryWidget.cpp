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

    if (QuickSlot) {
        QuickSlot->OnClicked.AddDynamic(this, &UInventoryWidget::OnQuickSlotClicked);
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
    if (!InventoryComponent || !InventorySlots || !InventorySlotWidgetClass) {
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
            InventorySlots->AddChildToUniformGrid(NewSlot, Row, Column);
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

    for (int32 Index = 0; Index < InventorySlotWidgets.Num(); Index++) {
        if (InventorySlotWidgets[Index]) {
            if (Index < InventoryComponent->InventoryItems.Num()) {
                InventorySlotWidgets[Index]->RefreshSlot(InventoryComponent->InventoryItems, Index);
            }
            else {
                InventorySlotWidgets[Index]->ClearSlot();
            }
        }
    }
}

void UInventoryWidget::RequestFocus(UInventorySlotWidget* NewfocusSlot)
{
    if (CurrentFocusedSlot == NewfocusSlot)
        return;

    if (CurrentFocusedSlot && CurrentFocusedSlot->IsFocused()) {
        CurrentFocusedSlot->SetIsFocused(false); //기존 포커스 제거
    }

    CurrentFocusedSlot = NewfocusSlot;

    if (CurrentFocusedSlot)
        CurrentFocusedSlot->SetIsFocused(true); //새 포커스 설정
}

void UInventoryWidget::MoveFocus(const FVector2D& Direction)
{
    if (InventorySlotWidgets.Num() == 0)
        return;

    const int32 Columns = InventoryColumnCount;
    const int32 TotalSlots = InventorySlotWidgets.Num();

    int32 CurrentIndex = InventorySlotWidgets.IndexOfByKey(CurrentFocusedSlot);
    if (CurrentIndex == INDEX_NONE) {
        RequestFocus(InventorySlotWidgets[0]);
        return;
    }

    int32 X = CurrentIndex % Columns;
    int32 Y = CurrentIndex / Columns;

    int32 NewX = FMath::Clamp(X + FMath::RoundToInt(Direction.X), 0, Columns - 1);
    int32 NewY = FMath::Clamp(Y - FMath::RoundToInt(Direction.Y), 0, (TotalSlots - 1) / Columns);

    int32 NewIndex = NewY * Columns + NewX;

    if (!InventorySlotWidgets.IsValidIndex(NewIndex)) 
        return;

    RequestFocus(InventorySlotWidgets[NewIndex]);
}

void UInventoryWidget::ConfirmFocusSlot()
{
    if (!CurrentFocusedSlot) 
        return;

    auto* ItemFactory = GetGameInstance()->GetSubsystem<UItemFactory>();
    if (!ItemFactory)
        return;

    const FInventoryItemData& FocusedData = CurrentFocusedSlot->GetCurrentInventoryItemData();

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
        case EItemType::Weapon:
        case EItemType::Armor:
        {
            auto* EquipableItem = Cast<AEquipableItem>(SpawnedItem);
            if (!EquipableItem)
                return;
            
            //PlayerCharacter->EquipItem(EquipableItem, 여기에 슬롯 어떻게 넣지?);
            break;
        }
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

void UInventoryWidget::OnQuickSlotClicked()
{
    if (InventoryItemData.ItemAmount <= 0)
        return;

    auto* PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!PlayerController)
        return;

    auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn());
    if (!PlayerCharacter)
        return;

    auto* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
    if (!PlayerHUD)
        return;

    auto* ItemFactory = GetGameInstance()->GetSubsystem<UItemFactory>();
    if (!ItemFactory)
        return;

    auto* InventoryComponent = PlayerCharacter->GetInventoryComponent();
    if (!InventoryComponent)
        return;

    const FInventoryItemData& FocusedData = CurrentFocusedSlot->GetCurrentInventoryItemData();
    if (ItemFactory->FindItemData(FocusedData.ItemTableID)->ItemType == EItemType::Consumable) {
        PlayerCharacter->AddToQuickSlot(1, FocusedData);
    }
}