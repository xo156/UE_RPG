// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryQuickSlotWidget.h"
#include "ItemBase.h"
#include "DataTableGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryQuickSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (QuickSlotAmount)
		QuickSlotAmount->SetText(FText::AsNumber(00));
}

void UInventoryQuickSlotWidget::SetQuickSlotConsumable(UTexture2D* NewQuickSlotItemIcon, int32 InventoryItemAmount)
{
	QuickSlotImage->SetBrushFromTexture(NewQuickSlotItemIcon);
	QuickSlotAmount->SetText(FText::AsNumber(InventoryItemAmount));

}
