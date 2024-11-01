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
		ItemTable = GameInstance->GetItemTable();
	}
}

void UInventoryItemAction::SetItemData(const FInventoryItemData& InItemData)
{
	InventoryItemData = InItemData;
}

void UInventoryItemAction::OnOnlyUseClicked()
{
	if (InventoryItemData.ItemAmount > 0) {
		if (ItemTable) {
			FItemData* ClickedItem = ItemTable->FindRow<FItemData>(FName(*FString::FromInt(InventoryItemData.ItemTableID)), TEXT("Item Data Context IvnentoryItemAction"));
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
	if (ItemTable) {
		FItemData* ClickedItem = ItemTable->FindRow<FItemData>(FName(*FString::FromInt(InventoryItemData.ItemTableID)), TEXT("Item Data Context IvnentoryItemAction"));
		switch (ClickedItem->ItemType) {
		case EItemType::Consumable:
			if (auto* ItemInstance = GetWorld()->SpawnActor<AItemBase>(ClickedItem->ItemClass[0])) {
				if (auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
					if (auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn())) {
						PlayerCharacter->SetQuickSlotItem(ItemInstance);
						PlayerCharacter->SetQuickSlotItemAmount(PlayerCharacter->GetInventory()->FindInventoryItem(ClickedItem->ItemID));
						if (InventoryQuickSlotWidgetClass) {
							InventoryQuickSlotWidgetInstance = CreateWidget<UInventoryQuickSlotWidget>(this, InventoryQuickSlotWidgetClass);
							if (InventoryQuickSlotWidgetInstance) {
								InventoryQuickSlotWidgetInstance->SetQuickSlotConsumable(ClickedItem->ItemIcon, PlayerCharacter->GetInventory()->FindInventoryItem(ClickedItem->ItemID));
							}
						}
						
					}
				}
			}
			break;
		default:
			break;
		}
	}

	//return 0,1,2,3 으로 바꿔보기
}
