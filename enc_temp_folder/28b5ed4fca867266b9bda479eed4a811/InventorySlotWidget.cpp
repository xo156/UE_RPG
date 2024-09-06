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

	if (Thumbnail555) {
		Thumbnail555->OnHovered.AddDynamic(this, &UInventorySlotWidget::OnThumbnailHovered);
		Thumbnail555->OnUnhovered.AddDynamic(this, &UInventorySlotWidget::OnThumbnailUnhovered);
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

void UInventorySlotWidget::RefreshSlot(TArray<FInventoryItemData> InventoryItem, int32 Index)
{
	CurrentInventoryItemData = InventoryItem[Index];
	UE_LOG(LogTemp, Log, TEXT("UInventorySlotWidget::RefreshSlot(): ItemID: %d, Amount: %d"), CurrentInventoryItemData.ItemTableID, CurrentInventoryItemData.ItemAmount);

	if (ItemDataTable) {
		static const FString ContextString(TEXT("Item Data Context"));
		FItemData* ItemData = ItemDataTable->FindRow<FItemData>(FName(*FString::FromInt(CurrentInventoryItemData.ItemTableID)), ContextString);
		if (ItemData) {
			FButtonStyle ButtonStyle = Thumbnail555->WidgetStyle;
			FSlateBrush NewBrush;
			NewBrush.SetResourceObject(ItemData->ItemIcon);
			ButtonStyle.SetNormal(NewBrush);
			ButtonStyle.SetHovered(NewBrush);
			ButtonStyle.SetPressed(NewBrush);
			Thumbnail555->SetStyle(ButtonStyle);

			AmountText->SetText(FText::AsNumber(CurrentInventoryItemData.ItemAmount));

			Thumbnail555->SetVisibility(ESlateVisibility::Visible);
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
	if (Thumbnail555 == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("InventorySlotWidget ClearSlot() Thumbnail is null"));
		
		return;
	}
	
	FButtonStyle ButtonStyle = Thumbnail555->WidgetStyle;
	FSlateBrush NewBrush;
	NewBrush.SetResourceObject(nullptr);
	ButtonStyle.SetNormal(NewBrush);
	ButtonStyle.SetHovered(NewBrush);
	ButtonStyle.SetPressed(NewBrush);
	Thumbnail555->SetStyle(ButtonStyle);

	AmountText->SetText(FText::AsNumber(0));

	Thumbnail555->SetVisibility(ESlateVisibility::Hidden);
	AmountText->SetVisibility(ESlateVisibility::Hidden);
}