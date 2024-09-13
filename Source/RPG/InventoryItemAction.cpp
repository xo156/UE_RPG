// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemAction.h"
#include "Components/Button.h"
#include "ItemBase.h"

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
	ItemData = InItemData;
}

void UInventoryItemAction::OnOnlyUseClicked()
{
	if (ItemData.ItemAmount > 0) {
		
	}
}

void UInventoryItemAction::OnOnlyDestroyClicked()
{
	if (ItemData.ItemAmount > 0) {
		
	}
}
