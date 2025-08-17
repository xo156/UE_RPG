// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "InventoryWidget.h"
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

	if (SlotThumbnail) {
		SlotThumbnail->OnHovered.AddDynamic(this, &UInventorySlotWidget::OnThumbnailHovered);
		SlotThumbnail->OnUnhovered.AddDynamic(this, &UInventorySlotWidget::OnUnThumbnailHovered);
		SlotThumbnail->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnThumbnailClicked);
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
		//TODO: ���� InventoryWidget�Ӹ� �ƴ϶� UEquipWidget������ �����ؾ� �Ѵ�
		//TODO: �׸��� ���� �ۼ��� �ڵ�� InventoryWidget�� ��ȿ�ϴ�
		//TODO: �׷��� UEquipWidget���� ��ȿ�� �ڵ带 �ۼ��ϰų� �ƿ� ���� ������ �Ѵ�
		ParentInventoryWidget->ConfirmFocusSlot();
	}
	return FReply::Handled();
}

void UInventorySlotWidget::RefreshSlot(TArray<FInventoryItemData> InventoryItem, int32 SlotIndex)
{
	CurrentInventoryItemData = InventoryItem[SlotIndex];

	auto* ItemFactory = GetGameInstance()->GetSubsystem<UItemFactory>();
	if (!ItemFactory)
		return;

	if (auto* ItemData = ItemFactory->FindItemData(CurrentInventoryItemData.ItemTableID)) {
		FButtonStyle ButtonStyle = SlotThumbnail->GetStyle();
		FSlateBrush NewBrush;
		NewBrush.SetResourceObject(ItemData->ItemIcon);

		ButtonStyle.SetNormal(NewBrush);
		ButtonStyle.SetHovered(NewBrush);
		ButtonStyle.SetPressed(NewBrush);
		SlotThumbnail->SetStyle(ButtonStyle);

		AmountText->SetText(FText::AsNumber(CurrentInventoryItemData.ItemAmount));

		SlotThumbnail->SetVisibility(ESlateVisibility::Visible);
		AmountText->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		ClearSlot();
	}
}

void UInventorySlotWidget::ClearSlot()
{	
	FButtonStyle ButtonStyle = SlotThumbnail->GetStyle();
	FSlateBrush NewBrush;
	NewBrush.SetResourceObject(nullptr);

	ButtonStyle.SetNormal(NewBrush);
	ButtonStyle.SetHovered(NewBrush);
	ButtonStyle.SetPressed(NewBrush);
	SlotThumbnail->SetStyle(ButtonStyle);

	AmountText->SetText(FText::AsNumber(0));

	AmountText->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlotWidget::SetParentInventoryWidget(UInventoryWidget* InWidget)
{
	if (InWidget) {
		ParentInventoryWidget = InWidget;
	}
}

void UInventorySlotWidget::SetIsFocused(bool bFocused)
{
	if (!FocusBorder)
		return;

	bIsFocused = bFocused;

	if (bIsFocused) {
		FocusBorder->SetBrushColor(FLinearColor::Red);
	}
	else {
		FocusBorder->SetBrushColor(DefaultBurshColor);
	}
}
