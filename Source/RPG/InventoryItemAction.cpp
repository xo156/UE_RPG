// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemAction.h"
#include "Components/Button.h"
#include "ItemBase.h"
#include "InventoryWidget.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"

void UInventoryItemAction::NativeConstruct()
{
	Super::NativeConstruct();

	if (OnlyUse) {
		OnlyUse->OnClicked.AddDynamic(this, &UInventoryItemAction::OnOnlyUseClicked);
	}

	if (OnlyDestroy) {
		OnlyDestroy->OnClicked.AddDynamic(this, &UInventoryItemAction::OnOnlyDestroyClicked);
	}
}

void UInventoryItemAction::SetItemData(const FInventoryItemData& InItemData)
{
	InventoryItemData = InItemData;
}

void UInventoryItemAction::OnOnlyUseClicked()
{
	if (InventoryItemData.ItemAmount > 0) {
		if (ItemDataTable) {
			FItemData* ClickedItem = ItemDataTable->FindRow<FItemData>(FName(*FString::FromInt(InventoryItemData.ItemTableID)), TEXT("Item Data Context"));
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
				//TODO: 이미 장착하고 있는 무기와 서로 위치 변경
				if (auto* ItemInstance = GetWorld()->SpawnActor<AItemBase>(ClickedItem->ItemClass[0])) {
					ItemInstance->Use();
					/*if (auto* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController())) {
						if (auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn())) {
							PlayerCharacter->GetInventory()->RemoveItem(InventoryItemData.ItemTableID, 1);
							PlayerCharacter->GetInventory()->InventoryWidget->UpdateInventoryWidget(PlayerCharacter->GetInventory());
						}
					}*/
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
		if (ItemDataTable) {
			FItemData* ClickedItem = ItemDataTable->FindRow<FItemData>(FName(*FString::FromInt(InventoryItemData.ItemTableID)), TEXT("Item Data Context"));
			switch (ClickedItem->ItemType) {
			case EItemType::Consumable:
				if (auto* ItemInstance = GetWorld()->SpawnActor<AItemBase>(ClickedItem->ItemClass[0])) {
					ItemInstance->DeleteItem();
					if (auto* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController())) {
						if (auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn())) {
							PlayerCharacter->GetInventory()->RemoveItem(InventoryItemData.ItemTableID, 1);
							PlayerCharacter->GetInventory()->InventoryWidget->UpdateInventoryWidget(PlayerCharacter->GetInventory());
						}
					}
				}
				break;
			case EItemType::Weapon:
				//TODO: 무기 세트로 날리기
				break;
			default:
				break;
			}
		}
	}
}