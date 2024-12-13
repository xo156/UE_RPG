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
		QuickSlotAmount->SetText(FText::AsNumber(0));

}

void UInventoryQuickSlotWidget::SetQuickSlotConsumable(UTexture2D* NewQuickSlotItemIcon, int32 InventoryItemAmount)
{
    if (QuickSlotImage && NewQuickSlotItemIcon) {
        QuickSlotImage->SetBrushFromTexture(NewQuickSlotItemIcon);
    }

    if (QuickSlotAmount) {
        QuickSlotAmount->SetText(FText::AsNumber(InventoryItemAmount));
    }
}

void UInventoryQuickSlotWidget::UpdateQuickSlotItemAmount(int32 NewAmount)
{
    UE_LOG(LogTemp, Log, TEXT("NewAmount is : %d"), NewAmount);
    if (QuickSlotAmount) {
        QuickSlotAmount->SetText(FText::AsNumber(NewAmount));
    }
}
