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
		UE_LOG(LogTemp, Error, TEXT("Thumbnail in NativeConstruct()"));
		Thumbnail->OnHovered.AddDynamic(this, &UInventorySlotWidget::OnThumbnailHovered);
		Thumbnail->OnUnhovered.AddDynamic(this, &UInventorySlotWidget::OnThumbnailUnhovered);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Thumbnail is null in NativeConstruct()"));
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
		InventoryTooltipInstance->RemoveFromParent();
		InventoryTooltipInstance = nullptr;
	}
}

void UInventorySlotWidget::RefreshSlot(FInventoryItemData InventoryItemData, int32 Index)
{
	UE_LOG(LogTemp, Log, TEXT("UInventorySlotWidget::RefreshSlot(): ItemID: %d, Amount: %d"), InventoryItemData.ItemTableID, InventoryItemData.ItemAmount);

	CurrentInventoryItemData = InventoryItemData;

	if (ItemDataTable) {
		static const FString ContextString(TEXT("Item Data Context"));
		FItemData* ItemData = ItemDataTable->FindRow<FItemData>(FName(*FString::FromInt(CurrentInventoryItemData.ItemTableID)), ContextString);
		if (ItemData) {
			FButtonStyle ButtonStyle = Thumbnail->WidgetStyle;
			FSlateBrush NewBrush;
			NewBrush.SetResourceObject(ItemData->ItemIcon);
			ButtonStyle.SetNormal(NewBrush);
			ButtonStyle.SetHovered(NewBrush);
			ButtonStyle.SetPressed(NewBrush);
			Thumbnail->SetStyle(ButtonStyle);

			AmountText->SetText(FText::AsNumber(InventoryItemData.ItemAmount));

			Thumbnail->SetVisibility(ESlateVisibility::Visible);
			AmountText->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("ItemData Not Found"));
			ClearSlot();
		}
	}
}

void UInventorySlotWidget::ClearSlot()
{
	if (Thumbnail == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("InventorySlotWidget ClearSlot() Thumbnail is null"));
		
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("ClearSlot() called for widget"));
	
	FButtonStyle ButtonStyle = Thumbnail->WidgetStyle;
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