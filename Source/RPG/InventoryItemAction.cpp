// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemAction.h"
#include "Components/Button.h"
#include "ItemBase.h"
#include "InventoryWidget.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "PlayerHUD.h"
#include "DropItem.h"
#include "DataTableGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "InventoryQuickSlotWidget.h"
#include "InventoryItemData.h"
#include "InventoryComponent.h"

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
}

void UInventoryItemAction::SetItemData(const FInventoryItemData& InItemData)
{
	InventoryItemData = InItemData;
}

FItemData* UInventoryItemAction::GetItemData() const
{
	if (auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
		return GameInstance->FindItemData(InventoryItemData.ItemTableID);
	}
	return nullptr;
}

void UInventoryItemAction::OnOnlyUseClicked()
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

void UInventoryItemAction::OnOnlyDestroyClicked()
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

void UInventoryItemAction::OnQuickSlotClicked()
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

			if (auto* QuickSlotWidget = PlayerHUD->GetQuickSlotWidget()) {
				QuickSlotWidget->SetQuickSlotConsumable(ItemData->ItemIcon, ItemAmount);
			}
		}
	}
}
