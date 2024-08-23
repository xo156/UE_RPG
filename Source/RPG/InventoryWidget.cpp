// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventoryComponent.h"
#include "InventorySlotWidget.h"
#include "Components/UniformGridPanel.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UInventoryWidget::CreateInventory(UInventoryComponent* InventoryComponent)
{
	if (!InventoryComponent || !InventorySlots || !InventorySlotWidgetClass)
		return;

	for (int32 Index = 0; Index < InventoryComponent->MaxSlotCounter; Index++) {
		UInventorySlotWidget* InventorySlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
		if (InventorySlot) {
			InventorySlots->AddChildToUniformGrid(InventorySlot, Index / 6, Index % 6);
		}
	}

}

void UInventoryWidget::UpdateInventory(UInventoryComponent* InventoryComponent)
{
	if (!InventoryComponent || !InventorySlots || !InventorySlotWidgetClass)
		return;

	for (int32 Index = 0; Index < InventoryComponent->MaxSlotCounter; Index++) {
		UInventorySlotWidget* InventorySlot = Cast<UInventorySlotWidget>(InventorySlots->GetChildAt(Index));
		if (InventorySlot->CurrentInventoryItemData.ItemAmount > 0) {
			InventorySlot->RefreshSlot(Index, InventorySlot->CurrentInventoryItemData);
		}
		else {
			InventorySlot->ClearSlot();
		}

	}
}
