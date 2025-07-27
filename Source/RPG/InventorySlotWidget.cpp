// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "DataTableGameInstance.h"
#include "ItemData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventoryTooltip.h"
#include "MyPlayerController.h"
#include "PlayerHUD.h"
#include "InventoryItemAction.h"
#include "Kismet/GameplayStatics.h"

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
		ItemCache = GameInstance->GetItemCache();
	}
}

void UInventorySlotWidget::RefreshSlot(TArray<FInventoryItemData> InventoryItem, int32 SlotIndex)
{
	CurrentInventoryItemData = InventoryItem[SlotIndex];
	if (ItemCache.Contains(CurrentInventoryItemData.ItemTableID)) {
		FItemData* ItemData = ItemCache.FindRef(CurrentInventoryItemData.ItemTableID);
		if (ItemData) {
			FButtonStyle ButtonStyle = Thumbnail->GetStyle();
			FSlateBrush NewBrush;
			NewBrush.SetResourceObject(ItemData->ItemIcon);

			ButtonStyle.SetNormal(NewBrush);
			ButtonStyle.SetHovered(NewBrush);
			ButtonStyle.SetPressed(NewBrush);
			Thumbnail->SetStyle(ButtonStyle);

			AmountText->SetText(FText::AsNumber(CurrentInventoryItemData.ItemAmount));

			Thumbnail->SetVisibility(ESlateVisibility::Visible);
			AmountText->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else {
		ClearSlot();
	}
}

void UInventorySlotWidget::ClearSlot()
{	
	FButtonStyle ButtonStyle = Thumbnail->GetStyle();
	FSlateBrush NewBrush;
	NewBrush.SetResourceObject(nullptr);

	ButtonStyle.SetNormal(NewBrush);
	ButtonStyle.SetHovered(NewBrush);
	ButtonStyle.SetPressed(NewBrush);
	Thumbnail->SetStyle(ButtonStyle);

	AmountText->SetText(FText::AsNumber(0));

	Thumbnail->SetVisibility(ESlateVisibility::Hidden);
	AmountText->SetVisibility(ESlateVisibility::Hidden);
}