// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "InventoryWidget.h"
#include "EquipWidget.h"
#include "ItemFactory.h"
#include "ItemData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "PlayerHUD.h"
#include "Kismet/GameplayStatics.h"

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotButton) {
		SlotButton->OnHovered.AddDynamic(this, &UInventorySlotWidget::OnThumbnailHovered);
		SlotButton->OnUnhovered.AddDynamic(this, &UInventorySlotWidget::OnUnThumbnailHovered);
		SlotButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnThumbnailClicked);
	}

	DefaultBurshColor = FocusBorder->GetBrushColor();
}

void UInventorySlotWidget::OnThumbnailHovered()
{
	FGeometry DummyGeometry;
	FPointerEvent DummyEvent;
	NativeOnMouseEnter(DummyGeometry, DummyEvent);
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (ParentInventoryWidget)
		ParentInventoryWidget->RequestFocus(this);
}

void UInventorySlotWidget::OnUnThumbnailHovered()
{
	FPointerEvent DummyEvent;
	NativeOnMouseLeave(DummyEvent);
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (ParentInventoryWidget)
		ParentInventoryWidget->RequestFocus(nullptr);
}

void UInventorySlotWidget::OnThumbnailClicked()
{
	FGeometry DummyGeometry;
	FPointerEvent DummyEvent;
	NativeOnMouseButtonDown(DummyGeometry, DummyEvent);
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (ParentInventoryWidget) {
		ParentInventoryWidget->RequestFocus(this);
		ParentInventoryWidget->ConfirmFocusSlot();
	}
	else if (ParentEquipWidget) {
		ParentEquipWidget->RequestFocus(this);
		ParentEquipWidget->HandleEquipInventorySlotClicked(this);
	}
	return FReply::Handled();
}

void UInventorySlotWidget::RefreshSlot(FInventoryItemData InventoryItem)
{
	CurrentInventoryItemData = InventoryItem;

	auto* ItemFactory = GetGameInstance()->GetSubsystem<UItemFactory>();
	if (!ItemFactory)
		return;

	if (auto* ItemData = ItemFactory->FindItemData(CurrentInventoryItemData.ItemTableID)) {
		SetItemIcon(ItemData->ItemIcon);

		AmountText->SetText(FText::AsNumber(CurrentInventoryItemData.ItemAmount));

		SlotButton->SetVisibility(ESlateVisibility::Visible);
		AmountText->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		ClearSlot();
	}
}

void UInventorySlotWidget::ClearSlot()
{	
	Super::ClearSlot();

	AmountText->SetText(FText::AsNumber(0));
	AmountText->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlotWidget::SetParentInventoryWidget(UInventoryWidget* InWidget)
{
	if (InWidget) {
		ParentInventoryWidget = InWidget;
	}
}

void UInventorySlotWidget::SetParentEquipWidget(UEquipWidget* InWidget)
{
	if (InWidget)
		ParentEquipWidget = InWidget;
}
