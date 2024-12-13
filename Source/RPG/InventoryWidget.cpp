// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventoryComponent.h"
#include "InventorySlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

    InventoryName->SetText(FText::FromString(TEXT("Inventory")));    
}

void UInventoryWidget::CreateInventoryWidget(UInventoryComponent* InventoryComponent)
{
    if (!InventoryComponent || !InventorySlots || !InventorySlotWidgetClass) {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Starting to create inventory with %d slots"), InventoryComponent->MaxSlotCounter);

    for (int32 Index = 0; Index < InventoryComponent->MaxSlotCounter; Index++) {
        InventorySlotWidgetInstance = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);

        if (InventorySlotWidgetInstance) {
            InventorySlots->AddChildToUniformGrid(InventorySlotWidgetInstance, Index / 6, Index % 6);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Inventory Widget creation completed."));
}

void UInventoryWidget::UpdateInventoryWidget(UInventoryComponent* InventoryComponent)
{
    if (!InventoryComponent || !InventorySlots || !InventorySlotWidgetClass) {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Starting to update inventory with %d slots"), InventoryComponent->MaxSlotCounter);

    for (int32 Index = 0; Index < InventoryComponent->MaxSlotCounter; Index++) {
        InventorySlotWidgetInstance = Cast<UInventorySlotWidget>(InventorySlots->GetChildAt(Index));

        if (InventorySlotWidgetInstance) {
            if (Index < InventoryComponent->InventoryItems.Num()) {
                InventorySlotWidgetInstance->RefreshSlot(InventoryComponent->InventoryItems, Index);
            }
            else {
                InventorySlotWidgetInstance->ClearSlot();
            }
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Failed to create widget instance at index %d"), Index);
        }
    }
}

