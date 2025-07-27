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
#include "InventoryItemData.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "PlayerHUD.h"
#include "DataTableGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

    //인벤토리 메인
    InventoryName->SetText(FText::FromString(TEXT("Inventory")));

    //툴 팁
    if (ItemName) {
        ItemName->SetText(FText::FromString("Item Name: (Default)"));
    }

    if (ItemValue) {
        ItemValue->SetText(FText::FromString("Item Value: (Default)"));
    }

    if (ItemDescription) {
        ItemDescription->SetText(FText::FromString("Item Description: (Default)"));
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

void UInventoryWidget::CreateInventorySlotWidget(UInventoryComponent* InventoryComponent)
{
    if (!InventoryComponent || !InventorySlots || !InventorySlotWidgetClass) {
        return;
    }

    InventorySlotWidgets.Empty();

    for (int32 Index = 0; Index < InventoryComponent->MaxSlotCounter; Index++) {
        auto* NewSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
        if (NewSlot) {
            InventorySlots->AddChildToUniformGrid(NewSlot, Index / 6, Index % 6);
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

void UInventoryWidget::InitTooltip(const FItemData& InItemData)
{
    if (ItemName) {
        ItemName->SetText(FText::FromString(InItemData.ItemName.ToString()));
    }

    if (ItemValue) {
        switch (InItemData.ItemType) {
        case EItemType::Consumable:
            ItemValue->SetText(FText::Format(FText::FromString(TEXT("회복량: {0}")), FText::AsNumber(InItemData.ItemRecoverValue)));
            break;
        case EItemType::Weapon:
            ItemValue->SetText(FText::Format(FText::FromString(TEXT("공격력: {0}")), FText::AsNumber(InItemData.ItemAttackValue)));
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

FItemData* UInventoryWidget::GetItemData() const
{
    if (auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
        return GameInstance->FindItemData(InventoryItemData.ItemTableID);
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

    if (auto* ItemData = GetItemData()) {
        if (auto* ItemInstance = GetWorld()->SpawnActor<AItemBase>(ItemData->ItemClass[0])) {
            ItemInstance->Use();

            auto* InventoryComponent = PlayerCharacter->GetInventoryComponent();
            InventoryComponent->RemoveItem(ItemData->ItemID, 1);
            InventoryComponent->InventoryWidget->UpdateInventorySlotWidget(InventoryComponent);

            //TODO: 플레이어 캐릭터에서 이 무기를 Equip
            if (ItemData->ItemType == EItemType::Weapon) {
                ItemInstance->Destroy();
            }
        }
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
    InventoryComponent->RemoveItem(InventoryItemData.ItemTableID, 1);
    InventoryComponent->InventoryWidget->UpdateInventorySlotWidget(InventoryComponent);
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

    if (auto* ItemData = GetItemData()) {
        if (ItemData->ItemType != EItemType::Consumable)
            return;

        if (auto* ItemInstance = GetWorld()->SpawnActor<AItemBase>(ItemData->ItemClass[0])) {
            PlayerCharacter->SetQuickSlotItem(ItemInstance);
            PlayerCharacter->SetQuickSlotItemID(ItemData->ItemID);

            int32 ItemAmount = PlayerCharacter->GetInventoryComponent()->GetInventoryItemAmount(ItemData->ItemID);
            PlayerCharacter->SetQuickSlotItemAmount(ItemAmount);

            /*if (auto* QuickSlotWidget = PlayerHUD->GetQuickSlotWidget()) {
                QuickSlotWidget->SetQuickSlotConsumable(ItemData->ItemIcon, ItemAmount);
            }*/
        }
    }
}