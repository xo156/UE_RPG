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
            UE_LOG(LogTemp, Log, TEXT("Successfully added slot at index %d"), Index);
            if (InventorySlotWidgetInstance->Thumbnail) {
                UE_LOG(LogTemp, Warning, TEXT("Thumbnail is vaild"));
            }
            else{
                UE_LOG(LogTemp, Warning, TEXT("Thumbnail is not vaild"));
            }
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
            UE_LOG(LogTemp, Log, TEXT("Widget instance created for slot index %d"), Index);

            // Thumbnail �ʱ�ȭ ���� �α�
            if (InventorySlotWidgetInstance->Thumbnail == nullptr) {
                UE_LOG(LogTemp, Warning, TEXT("Thumbnail is null at slot index %d"), Index);
            }
            else {
                UE_LOG(LogTemp, Log, TEXT("Thumbnail is valid at slot index %d"), Index);
            }

            if (Index < InventoryComponent->InventoryItems.Num()) {
                InventorySlotWidgetInstance->RefreshSlot(InventorySlotWidgetInstance->CurrentInventoryItemData, Index);
            }
            else {
                UE_LOG(LogTemp, Log, TEXT("Clearing slot at index %d"), Index);
                InventorySlotWidgetInstance->ClearSlot();
            }
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Failed to create widget instance at index %d"), Index);
        }
    }
}

