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
		FLinearColor RedColor(1.0f, 0.0f, 0.0f, 1.0f);
		FButtonStyle ButtonStyle = Thumbnail->WidgetStyle;
		FSlateBrush RedBrush;
		RedBrush.TintColor = FSlateColor(RedColor);
		Thumbnail->WidgetStyle = ButtonStyle;

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

void UInventorySlotWidget::RefreshSlot(FInventoryItemData InventoryItemData)
{
	UE_LOG(LogTemp, Log, TEXT("Refreshing slot with SlotIndex: ItemID: %d, Amount: %d"), InventoryItemData.ItemTableID, InventoryItemData.ItemAmount);

	CurrentInventoryItemData = InventoryItemData;

	if (ItemDataTable) {
		static const FString ContextString(TEXT("Item Data Context"));
		FItemData* ItemData = ItemDataTable->FindRow<FItemData>(FName(*FString::FromInt(CurrentInventoryItemData.ItemTableID)), ContextString);
		if (ItemData) {
			FButtonStyle ButtonStyle = Thumbnail->WidgetStyle;
			FSlateBrush NewBrush;
			NewBrush.SetResourceObject(ItemData->ItemIcon);
			ButtonStyle.SetNormal(NewBrush);
			Thumbnail->SetStyle(ButtonStyle);

			AmountText->SetText(FText::AsNumber(InventoryItemData.ItemAmount));

			Thumbnail->SetVisibility(ESlateVisibility::Visible);
			AmountText->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UInventorySlotWidget::ClearSlot()
{
	if (Thumbnail == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Thumbnail is null"));
		return;
	}	
	
	FButtonStyle ButtonStyle = Thumbnail->WidgetStyle;
	FSlateBrush NewBrush;
	NewBrush.SetResourceObject(nullptr);
	ButtonStyle.SetNormal(NewBrush);
	Thumbnail->SetStyle(ButtonStyle);

	AmountText->SetText(FText::AsNumber(0));

	Thumbnail->SetVisibility(ESlateVisibility::Hidden);
	AmountText->SetVisibility(ESlateVisibility::Hidden);
}
