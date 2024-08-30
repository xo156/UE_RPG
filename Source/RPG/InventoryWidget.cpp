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
    if (!InventoryComponent) {
        UE_LOG(LogTemp, Warning, TEXT("InventoryComponent is null."));
        return;
    }

    if (!InventorySlots) {
        UE_LOG(LogTemp, Warning, TEXT("InventorySlots is null."));
        return;
    }

    if (!InventorySlotWidgetClass) {
        UE_LOG(LogTemp, Warning, TEXT("InventorySlotWidgetClass is null."));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Starting to create inventory with %d slots"), InventoryComponent->MaxSlotCounter);

    for (int32 Index = 0; Index < InventoryComponent->MaxSlotCounter; Index++) {
        InventorySlotWidgetInstance = CreateWidget<UInventorySlotWidget>(GetWorld(), InventorySlotWidgetClass);

        if (InventorySlotWidgetInstance) {
            InventorySlots->AddChildToUniformGrid(InventorySlotWidgetInstance, Index / 6, Index % 6);

            UE_LOG(LogTemp, Log, TEXT("Successfully added slot at index %d"), Index);
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Failed to create InventorySlotWidgetInstance at index %d"), Index);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Inventory creation completed."));
}

void UInventoryWidget::UpdateInventory(UInventoryComponent* InventoryComponent)
{
    if (!InventoryComponent) {
        UE_LOG(LogTemp, Warning, TEXT("InventoryComponent is null."));
        return;
    }

    if (!InventorySlots) {
        UE_LOG(LogTemp, Warning, TEXT("InventorySlots is null."));
        return;
    }

    if (!InventorySlotWidgetClass) {
        UE_LOG(LogTemp, Warning, TEXT("InventorySlotWidgetClass is null."));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Starting to update inventory with %d slots"), InventoryComponent->MaxSlotCounter);

	for (int32 Index = 0; Index < InventoryComponent->MaxSlotCounter; Index++) {
		InventorySlotWidgetInstance = Cast<UInventorySlotWidget>(InventorySlots->GetChildAt(Index));
		if (InventorySlotWidgetInstance->CurrentInventoryItemData.ItemAmount > 0) {
			InventorySlotWidgetInstance->RefreshSlot(InventorySlotWidgetInstance->CurrentInventoryItemData);
		}
		else {
			InventorySlotWidgetInstance->ClearSlot();
		}
	}
}
