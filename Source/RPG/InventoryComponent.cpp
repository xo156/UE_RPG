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

	MaxSlotCounter = 36;
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

bool UInventoryComponent::TryAddItem(ADropItem* AddedItem)
{
	if (AddedItem) {
		if (CurrentSlotCounter >= MaxSlotCounter) {
			UE_LOG(LogTemp, Error, TEXT("Inventory Full"));
			return false;
		}

		if (AddedItem->DropItemData.bCounterble) {
			int32 SlotIndex = FindSlotIndex(AddedItem);
			if (SlotIndex != INDEX_NONE) {
				StackItem(AddedItem, SlotIndex);
				return true;
			}
			else {
				AddItem(AddedItem);
				return true;
			}
		}
		else {
			AddItem(AddedItem);
			return true;
		}
	}
	return false;
}

int32 UInventoryComponent::FindSlotIndex(ADropItem* AddItem)
{
	int32 Index = -1;
	for (int32 i = 0; i < InventoryItems.Num(); i++) {
		if (AddItem->DropItemData.ItemID == InventoryItems[i].ItemTableID) {
            UE_LOG(LogTemp, Log, TEXT("Can Find Slot Index : %d"), i);
			return i;
		}
	}
	return INDEX_NONE;
}

void UInventoryComponent::StackItem(ADropItem* AddedItem, int32 SlotIndex)
{
	if (InventoryItems.IsValidIndex(SlotIndex) && AddedItem) {
		FInventoryItemData& SlotItemData = InventoryItems[SlotIndex];

        //SlotItemData.ItemUID = MakeUID();
		SlotItemData.ItemAmount += AddedItem->DropItemData.Amount;
        SlotItemData.ItemTableID = AddedItem->DropItemData.ItemID;
        SlotItemData.bCounterble = AddedItem->DropItemData.bCounterble;

        UE_LOG(LogTemp, Log, TEXT("Stack Item With UID: %d, ItemID: %d, Amount: %d, Couterble: %s"),
               SlotItemData.ItemUID, SlotItemData.ItemTableID, SlotItemData.ItemAmount, 
               SlotItemData.bCounterble ? TEXT("True") : TEXT("False"));

        UE_LOG(LogTemp, Log, TEXT("Current inventory size: %d"), InventoryItems.Num());
	}
}

void UInventoryComponent::AddItem(ADropItem* AddedItem)
{
	if (AddedItem) {
		FInventoryItemData NewInventoryItemData;
		NewInventoryItemData.ItemUID = MakeUID();
		NewInventoryItemData.ItemAmount = AddedItem->DropItemData.Amount;
		NewInventoryItemData.ItemTableID = AddedItem->DropItemData.ItemID;
		NewInventoryItemData.bCounterble = AddedItem->DropItemData.bCounterble;

		UE_LOG(LogTemp, Log, TEXT("Add Item With UID: %d, ItemID: %d, Amount: %d, Couterble: %s"),
			   NewInventoryItemData.ItemUID, NewInventoryItemData.ItemTableID, 
			   NewInventoryItemData.ItemAmount,
			   NewInventoryItemData.bCounterble ? TEXT("True") : TEXT("False"));

		InventoryItems.Add(NewInventoryItemData);
		for (FInventoryItemData CheckItem : InventoryItems) {
			UE_LOG(LogTemp, Log, TEXT("CheckItem: %d"), CheckItem.ItemTableID);
		}
		CurrentSlotCounter++;
		UE_LOG(LogTemp, Log, TEXT("Current inventory size: %d"), InventoryItems.Num());
	}

}

int32 UInventoryComponent::MakeUID()
{
	return ++UIDCounter;
}

void UInventoryComponent::CreateInventoryWidget()
{
	if (InventoryWidgetClass) {
		InventoryWidget = CreateWidget<UInventoryWidget>(UGameplayStatics::GetPlayerController(this, 0), InventoryWidgetClass);
		if (InventoryWidget) {
            InventoryWidget->CreateInventoryWidget(this);
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UInventoryComponent::OpenInventoryWidget()
{
	if (InventoryWidget && !bIsOpen) {
        UE_LOG(LogTemp, Error, TEXT("UInventoryComponent::OpenInventoryWidget()"));
		InventoryWidget->UpdateInventoryWidget(this);
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;
		bIsOpen = true;
	}
}

void UInventoryComponent::CloseInventoryWidget()
{
	if (InventoryWidget && bIsOpen) {
        UE_LOG(LogTemp, Error, TEXT("CloseInventoryWidget()"));
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = false;
		bIsOpen = false;
	}
}
