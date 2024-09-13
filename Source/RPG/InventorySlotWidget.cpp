// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "InventoryItemData.h"
#include "ItemData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventoryTooltip.h"
#include "MyPlayerController.h"
#include "InventoryItemAction.h"

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
	if (auto* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController())) {
		if (InventoryTooltipClass) {
			InventoryTooltipInstance = CreateWidget<UInventoryTooltip>(this, InventoryTooltipClass);
			if (InventoryTooltipInstance){
				if (ItemDataTable) {
					FItemData* ShowedData = ItemDataTable->FindRow<FItemData>(FName(*FString::FromInt(CurrentInventoryItemData.ItemTableID)), TEXT("Item Data Context"));
					if (ShowedData) {
						InventoryTooltipInstance->InitTooltip(*ShowedData);
					}
				}
				PlayerController->ShowTooltipAtMousePosition(InventoryTooltipInstance);
			}
		}
	}
	bIsHover = true;
}

void UInventorySlotWidget::OnThumbnailUnhovered()
{
	if (auto* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController())) {
		PlayerController->HideTooltip();
	}
	bIsHover = false;
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton && bIsHover) {
		if (auto* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController())) {
			if (InventoryItemActionClass) {
				InventoryItemActionInstance = CreateWidget<UInventoryItemAction>(this, InventoryItemActionClass);
				InventoryItemActionInstance->SetItemData(CurrentInventoryItemData);
				PlayerController->ShotItemActionMousePosition(InventoryItemActionInstance);
			}
		}
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UInventorySlotWidget::RefreshSlot(TArray<FInventoryItemData> InventoryItem, int32 Index)
{
	CurrentInventoryItemData = InventoryItem[Index];
	UE_LOG(LogTemp, Log, TEXT("UInventorySlotWidget::RefreshSlot(): ItemID: %d, Amount: %d"), CurrentInventoryItemData.ItemTableID, CurrentInventoryItemData.ItemAmount);

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

			AmountText->SetText(FText::AsNumber(CurrentInventoryItemData.ItemAmount));

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