// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemAction.h"
#include "Components/Button.h"
#include "ItemBase.h"
#include "InventoryWidget.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "DropItem.h"
#include "DataTableGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "InventoryQuickSlotWidget.h"
#include "InventoryItemData.h"

void UInventoryItemAction::NativeConstruct()
{
	Super::NativeConstruct();

	if (OnlyUse) {
		OnlyUse->OnClicked.AddDynamic(this, &UInventoryItemAction::OnOnlyUseClicked);
	}

	if (OnlyDestroy) {
		OnlyDestroy->OnClicked.AddDynamic(this, &UInventoryItemAction::OnOnlyDestroyClicked);
	}

	if (QuickSlot) {
		QuickSlot->OnClicked.AddDynamic(this, &UInventoryItemAction::OnQuickSlotClicked);
	}

	if (auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
		ItemCache = GameInstance->GetItemCache();
	}
}

void UInventoryItemAction::SetItemData(const FInventoryItemData& InItemData)
{
	InventoryItemData = InItemData;
}

void UInventoryItemAction::OnOnlyUseClicked()
{
	if (InventoryItemData.ItemAmount > 0) {
		auto* ClickedItem = ItemCache.FindRef(InventoryItemData.ItemTableID);
		if (ClickedItem) {
			switch (ClickedItem->ItemType) {
			case EItemType::Consumable:
				if (auto* ItemInstance = GetWorld()->SpawnActor<AItemBase>(ClickedItem->ItemClass[0])) {
					ItemInstance->Use();
					if (auto* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController())) {
						if (auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn())) {
							PlayerCharacter->GetInventory()->RemoveItem(InventoryItemData.ItemTableID, 1);
							PlayerCharacter->GetInventory()->InventoryWidget->UpdateInventoryWidget(PlayerCharacter->GetInventory());
						}
					}
				}
				break;
			case EItemType::Weapon:
				if (auto* ItemInstance = GetWorld()->SpawnActor<AItemBase>(ClickedItem->ItemClass[0])) {
					if (auto* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController())) {
						if (auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn())) {
							ItemInstance->Use();
							PlayerCharacter->GetInventory()->RemoveItem(InventoryItemData.ItemTableID, 1);
							PlayerCharacter->GetInventory()->InventoryWidget->UpdateInventoryWidget(PlayerCharacter->GetInventory());
							ItemInstance->Destroy();
						}
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

void UInventoryItemAction::OnOnlyDestroyClicked()
{
	if (InventoryItemData.ItemAmount > 0) {
		if (auto* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController())) {
			if (auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn())) {
				PlayerCharacter->GetInventory()->RemoveItem(InventoryItemData.ItemTableID, 1);
				PlayerCharacter->GetInventory()->InventoryWidget->UpdateInventoryWidget(PlayerCharacter->GetInventory());
			}
		}
	}
}

void UInventoryItemAction::OnQuickSlotClicked()
{
	if (ItemCache.Contains(InventoryItemData.ItemTableID)) {
		FItemData* ClickedItem = ItemCache.FindRef(InventoryItemData.ItemTableID);
		if (ClickedItem) {
			switch (ClickedItem->ItemType) {
			case EItemType::Consumable:
				if (auto* ItemInstance = GetWorld()->SpawnActor<AItemBase>(ClickedItem->ItemClass[0])) {
					if (auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
						if (auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn())) {
							PlayerCharacter->SetQuickSlotItem(ItemInstance);
							int32 ItemAmount = PlayerCharacter->GetInventory()->GetInventoryItemAmount(ClickedItem->ItemID);
							if (InventoryQuickSlotWidgetClass) {
								if (!InventoryQuickSlotWidgetInstance) {
									InventoryQuickSlotWidgetInstance = CreateWidget<UInventoryQuickSlotWidget>(this, InventoryQuickSlotWidgetClass);
									if (InventoryQuickSlotWidgetInstance) {
										InventoryQuickSlotWidgetInstance->AddToViewport();
									}
								}
								InventoryQuickSlotWidgetInstance->SetQuickSlotConsumable(ClickedItem->ItemIcon, ItemAmount);
								PlayerCharacter->SetQuickSlotItemID(ClickedItem->ItemID);
								PlayerCharacter->SetQuickSlotItemAmount(ItemAmount);
							}
						}
					}
				}
				break;
			default:
				break;
			}
		}
	}
}
