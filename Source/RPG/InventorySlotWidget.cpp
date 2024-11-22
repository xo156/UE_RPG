// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "InventoryItemData.h"
#include "DataTableGameInstance.h"
#include "ItemData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventoryTooltip.h"
#include "MyPlayerController.h"
#include "InventoryItemAction.h"
#include "Kismet/GameplayStatics.h"

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Thumbnail) {
		Thumbnail->OnHovered.AddDynamic(this, &UInventorySlotWidget::OnThumbnailHovered);
		Thumbnail->OnUnhovered.AddDynamic(this, &UInventorySlotWidget::OnThumbnailUnhovered);
	}

	if (auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
		ItemCache = GameInstance->GetItemCache();
	}
}

void UInventorySlotWidget::OnThumbnailHovered()
{
	if (auto* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController())) {
		if (InventoryTooltipClass) {
			InventoryTooltipInstance = CreateWidget<UInventoryTooltip>(this, InventoryTooltipClass);
			if (InventoryTooltipInstance) {
				auto* ShowedData = ItemCache.FindRef(CurrentInventoryItemData.ItemTableID); // 캐시에서 찾기
				if (ShowedData) {
					InventoryTooltipInstance->InitTooltip(*ShowedData);
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

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton && bIsHover) { //우클릭하면 사용 혹은 버리기 선택
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

void UInventorySlotWidget::RefreshSlot(TArray<FInventoryItemData> InventoryItem, int32 SlotIndex)
{
	CurrentInventoryItemData = InventoryItem[SlotIndex];
	if (ItemCache.Contains(CurrentInventoryItemData.ItemTableID)) {
		FItemData* ItemData = ItemCache.FindRef(CurrentInventoryItemData.ItemTableID);
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
	}
	else {
		ClearSlot();
	}
}

void UInventorySlotWidget::ClearSlot()
{	
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