// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "DropItem.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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
	for (int32 i = 0; i < Inventory.Num(); i++) {
		if (AddItem->DropItemData.ItemID == Inventory[i].ItemTableID) {
			return i;
		}
	}
	
	return INDEX_NONE;
}

void UInventoryComponent::StackItem(ADropItem* AddItem, int32 SlotIndex)
{
	if (Inventory.IsValidIndex(SlotIndex)) {
		FInventoryItemData& SlotItemData = Inventory[SlotIndex];

		SlotItemData.ItemAmount += AddItem->DropItemData.Amount;
	}
}

void UInventoryComponent::AddItem(ADropItem* AddItem)
{
	if (AddItem) {
		if (AddItem->DropItemData.bCounterble) {
			int32 SlotIndex = FindSlotIndex(AddItem);
			if (SlotIndex != INDEX_NONE) {
				StackItem(AddItem, SlotIndex);
			}
		}
		else {
			if (MaxSlotCounter > CurrentSlotCounter) {
				FInventoryItemData NewInventoryItemData;

				NewInventoryItemData.ItemUID = MakeUID();
				NewInventoryItemData.ItemTableID = AddItem->DropItemData.ItemID;
				NewInventoryItemData.ItemAmount = AddItem->DropItemData.Amount;
				NewInventoryItemData.bCounterble = AddItem->DropItemData.bCounterble;

				Inventory.Add(NewInventoryItemData);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Inventory Is Full"));
			}
		}
		
	}
}

int32 UInventoryComponent::MakeUID()
{
	return UIDCounter++;
}
