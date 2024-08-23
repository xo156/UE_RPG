// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "InventoryItemData.h"
#include "ItemData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventoryTooltip.h"

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Thumbnail) {
		Thumbnail->OnHovered.AddDynamic(this, &UInventorySlotWidget::OnThumbnailHovered);
		Thumbnail->OnUnhovered.AddDynamic(this, &UInventorySlotWidget::OnThumbnailUnhovered);
	}
}

void UInventorySlotWidget::OnThumbnailHovered()
{
	if (InventoryTooltipClass) {
		InventoryTooltipInstance = CreateWidget<UInventoryTooltip>(GetWorld(), InventoryTooltipClass);
		if (InventoryTooltipInstance) {
			InventoryTooltipInstance->SetVisibility(ESlateVisibility::Visible);
			InventoryTooltipInstance->AddToViewport();
		}
	}
}

void UInventorySlotWidget::OnThumbnailUnhovered()
{
	if (InventoryTooltipInstance) {
		InventoryTooltipInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventorySlotWidget::RefreshSlot(int32 SlotIndex, FInventoryItemData InventoryItemData)
{
	CurrentSlotIndex = SlotIndex;
	CurrentInventoryItemData = InventoryItemData;

	if (ItemDataTable) {
		static const FString ContextString(TEXT("Item Data Context"));
		FItemData* ItemData = ItemDataTable->FindRow<FItemData>(FName(*FString::FromInt(CurrentInventoryItemData.ItemTableID)), ContextString);
		if (ItemData) {
			FButtonStyle ButtonStyle = Thumbnail->WidgetStyle;
			ButtonStyle.Normal.SetResourceObject(ItemData->ItemIcon);
			Thumbnail->SetStyle(ButtonStyle);

			AmountText->SetText(FText::AsNumber(InventoryItemData.ItemAmount));
		}
	}
}

void UInventorySlotWidget::ClearSlot()
{
	FButtonStyle ButtonStyle = Thumbnail->WidgetStyle;
	//TODO: visibilty¸¦ hiddenÀ¸·Î

	AmountText->SetText(FText::AsNumber(00));
}


