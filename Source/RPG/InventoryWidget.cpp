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

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

    //�κ��丮 ����
    InventoryName->SetText(FText::FromString(TEXT("Inventory")));

    //�� ��
    if (ItemName) {
        ItemName->SetText(FText::FromString("Item Name: (Default)"));
    }

    if (ItemValue) {
        ItemValue->SetText(FText::FromString("Item Value: (Default)"));
    }

    if (ItemDescription) {
        ItemDescription->SetText(FText::FromString("Item Description: (Default)"));
    }

    //������ �׼�
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
        CurrentFocusedSlot->SetIsFocused(false); //���� ��Ŀ�� ����
    }

    CurrentFocusedSlot = NewfocusSlot;

    if (CurrentFocusedSlot)
        CurrentFocusedSlot->SetIsFocused(true); //�� ��Ŀ�� ����
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

    const FInventoryItemData& FocusedData = CurrentFocusedSlot->GetCurrentInventoryItemData();

    bool bValidItem = FocusedData.ItemAmount > 0 || FocusedData.bRemainInInventoryWhenAmountZero;
    if (bValidItem) {
        const FItemData* ItemData = GetItemDataByID(FocusedData.ItemTableID);
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

    if (ItemValue) {
        switch (InItemData.ItemType) {
        case EItemType::Consumable:
            //ItemValue->SetText(FText::Format(FText::FromString(TEXT("ȸ����: {0}")), FText::AsNumber(InItemData.ItemRecoverValue)));
            break;
        case EItemType::Weapon:
            //ItemValue->SetText(FText::Format(FText::FromString(TEXT("���ݷ�: {0}")), FText::AsNumber(InItemData.ItemAttackValue)));
            break;
        default:
            ItemValue->SetText(FText::FromString(TEXT("N/A")));
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

    if (ItemValue) {
        ItemValue->SetText(FText::FromString(""));
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

FItemData* UInventoryWidget::GetItemDataByID(int32 ItemTableID) const
{
    if (auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
        return GameInstance->FindItemData(ItemTableID);
    }
    return nullptr;
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

    auto* SpawnedItem = ItemFactory->SpawnItemFromTableID(GetWorld(), InventoryItemData.ItemTableID);
    if (SpawnedItem) {
        switch (SpawnedItem->GetItemType())
        {
        case EItemType::Weapon:
        case EItemType::Armor:
            //TODO: �÷��̾� ĳ���Ϳ��� �� ���⸦ Equip
            //������ �����ϰ� �ִ� ����� �κ��丮��
            //�� �Ǽ��� �κ��丮�� ���� �ʴ´�
            //�ٵ� �̰� �� ���� ������ �ϴ°ǰ�?
            break;
        case EItemType::Consumable:
        case EItemType::Quest:
            //TODO: �̰� ������ ����ϴ°Ÿ� �������̽��� ���� ���� ����
            //����Ʈ �����ۿ��� ����ϴ� ��� ������ ���� ����
        default:
            break;
        }
        auto* InventoryComponent = PlayerCharacter->GetInventoryComponent();
        InventoryComponent->RemoveItem(InventoryItemData.ItemTableID, 1);
        InventoryComponent->GetInventoryWidget()->UpdateInventorySlotWidget(InventoryComponent);

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

    auto* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
    if (!PlayerHUD)
        return;

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

    /*const auto* ItemData = GetItemDataByID(InventoryItemData.ItemTableID);
    if (!ItemData)
        return;

    if (ItemData->ItemType != EItemType::Consumable)
        return;

    if (auto* ItemInstance = GetWorld()->SpawnActor<AItemBase>(ItemData->ItemClass[0])) {
        PlayerCharacter->SetQuickSlotItem(ItemInstance);
        PlayerCharacter->SetQuickSlotItemID(ItemData->ItemTableID);

        int32 ItemAmount = PlayerCharacter->GetInventoryComponent()->GetInventoryItemAmount(ItemData->ItemTableID);
        PlayerCharacter->SetQuickSlotItemAmount(ItemAmount);

        if (auto* QuickSlotWidget = PlayerHUD->GetQuickSlotWidget()) {
            QuickSlotWidget->SetQuickSlotConsumable(ItemData->ItemIcon, ItemAmount);
        }
    }*/
}