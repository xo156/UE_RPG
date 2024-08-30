// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "DropItem.h"
#include "InventoryWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxSlotCounter = 20;
	CurrentSlotCounter = 0;
	UIDCounter = 0;
    bIsOpen = false;
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UInventoryComponent::FindSlotIndex(ADropItem* AddItem)
{
	int32 Index = -1;
	for (int32 i = 0; i < InventoryItems.Num(); i++) {
		if (AddItem->DropItemData.ItemID == InventoryItems[i].ItemTableID) {
			return i;
		}
	}
	
	return INDEX_NONE;
}

void UInventoryComponent::StackItem(ADropItem* AddItem, int32 SlotIndex)
{
	if (InventoryItems.IsValidIndex(SlotIndex)) {
		FInventoryItemData& SlotItemData = InventoryItems[SlotIndex];

		SlotItemData.ItemAmount += AddItem->DropItemData.Amount;
	}
}

bool UInventoryComponent::AddItem(ADropItem* AddItem)
{
    if (AddItem) {
        if (AddItem->DropItemData.bCounterble) {
            int32 SlotIndex = FindSlotIndex(AddItem);
            if (SlotIndex != INDEX_NONE) {
                StackItem(AddItem, SlotIndex);

                FInventoryItemData NewInventoryItemData;

                NewInventoryItemData.ItemUID = MakeUID();
                NewInventoryItemData.ItemTableID = AddItem->DropItemData.ItemID;
                NewInventoryItemData.ItemAmount = AddItem->DropItemData.Amount;
                NewInventoryItemData.bCounterble = AddItem->DropItemData.bCounterble;

                UE_LOG(LogTemp, Log, TEXT("Adding item with UID: %d, ItemID: %d, Amount: %d, IsCountable: %s"),
                       NewInventoryItemData.ItemUID, NewInventoryItemData.ItemTableID,
                       NewInventoryItemData.ItemAmount,
                       NewInventoryItemData.bCounterble ? TEXT("True") : TEXT("False"));

                InventoryItems.Add(NewInventoryItemData);
                if (InventoryWidget) {
                    InventoryWidget->UpdateInventory(this);
                }
                UE_LOG(LogTemp, Log, TEXT("Current inventory size: %d"), InventoryItems.Num());

                return true;
            }
        }
        else {
            if (MaxSlotCounter > CurrentSlotCounter) {
                FInventoryItemData NewInventoryItemData;

                NewInventoryItemData.ItemUID = MakeUID();
                NewInventoryItemData.ItemTableID = AddItem->DropItemData.ItemID;
                NewInventoryItemData.ItemAmount = AddItem->DropItemData.Amount;
                NewInventoryItemData.bCounterble = AddItem->DropItemData.bCounterble;

                UE_LOG(LogTemp, Log, TEXT("Adding item with UID: %d, ItemID: %d, Amount: %d, IsCountable: %s"),
                       NewInventoryItemData.ItemUID, NewInventoryItemData.ItemTableID,
                       NewInventoryItemData.ItemAmount,
                       NewInventoryItemData.bCounterble ? TEXT("True") : TEXT("False"));

                InventoryItems.Add(NewInventoryItemData);
                if (InventoryWidget) {
                    InventoryWidget->UpdateInventory(this);
                }
                UE_LOG(LogTemp, Log, TEXT("Current inventory size: %d"), InventoryItems.Num());

                return true;
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("Inventory Is Full"));
                return false;
            }
        }
    }
    return false;
}

int32 UInventoryComponent::MakeUID()
{
	return ++UIDCounter;
}

void UInventoryComponent::CreateInventoryWidget()
{
    UE_LOG(LogTemp, Log, TEXT("UInventoryComponent::CreateInventoryWidget()"));
	if (InventoryWidgetClass) {
		InventoryWidget = CreateWidget<UInventoryWidget>(UGameplayStatics::GetPlayerController(this, 0), InventoryWidgetClass);
		if (InventoryWidget) {
            UE_LOG(LogTemp, Error, TEXT("Inventory Widget Create"));
            InventoryWidget->CreateInventory(this);

			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UInventoryComponent::OpenInventoryWidget()
{
	if (InventoryWidget && !bIsOpen) {
        UE_LOG(LogTemp, Error, TEXT("Inventory Widget Open"));
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;
		bIsOpen = true;
	}
}

void UInventoryComponent::CloseInventoryWidget()
{
	if (InventoryWidget && bIsOpen) {
        UE_LOG(LogTemp, Error, TEXT("Inventory Widget Close"));
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = false;
		bIsOpen = false;
	}
}
