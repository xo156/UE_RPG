// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryQuickSlotWidget.h"
#include "ItemBase.h"
#include "DataTableGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"

void UInventoryQuickSlotWidget::NativeConstruct()
{
	if (auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
		ItemTable = GameInstance->GetItemTable();
	}
}

void UInventoryQuickSlotWidget::SetQuickSlotThumbnail(UTexture2D* NewQuickSlotItemIcon)
{
	if (NewQuickSlotItemIcon && QuickSlotThumbnail) {
		QuickSlotThumbnail->SetBrushFromTexture(NewQuickSlotItemIcon);
	}
	else {
		QuickSlotThumbnail->SetBrushFromTexture(nullptr);
	}
}
