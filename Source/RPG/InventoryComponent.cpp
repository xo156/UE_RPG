// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemBase.h"
#include "ItemDrop.h"
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

void UInventoryComponent::AddItem(TArray<ADropItem*> Items)
{
	for (auto* Item : Items) {
		if (Inventory.Contains(Item->GetItemDrop().ItemID)) { //contain 안됨
			if (auto* ItemBase = Cast<AItemBase>(Item)) { //cast 의미 없음
				if (Item->GetItemDrop().bIsCountable) {
					if (GetInventoryItemAmount(Item) <= GetInventoryItem(Item)->ItemMaxAmount) {
						GetInventoryItem(Item)->ItemCurrentAmount++;
					}
					else {
						Inventory[Inventory.Num()] = Item;
					}
				}
				else {
					Inventory[Inventory.Num()] = Item;
				}
			}
		}
		else {

		}
	}
}

void UInventoryComponent::UseItem(ADropItem* InventoryItem, int32 Quantity)
{
	if (GetInventoryItem(InventoryItem)->ItemMaxAmount <= Quantity && Quantity < 0) {
		for (int32 UseItemCount = 0; UseItemCount < Quantity; UseItemCount++) {
			if (auto* Item = Cast<AItemBase>(InventoryItem)) {
				Item->Use();
			}
		}
	}
}

void UInventoryComponent::RemoveItem(ADropItem* InventoryItem, int32 Quantity)
{
	if (GetInventoryItem(InventoryItem)->ItemMaxAmount <= Quantity && Quantity < 0) {
		for (int32 RemoveItemCount = 0; RemoveItemCount < Quantity; RemoveItemCount++) {
			GetInventoryItem(InventoryItem)->ItemCurrentAmount--;
		}
	}
}


int32 UInventoryComponent::GetInventoryItemAmount(ADropItem* Item)
{
	if (Item)
		return Item->GetItemDrop().DropItemAmount;
}

FInventoryItemData* UInventoryComponent::GetInventoryItem(ADropItem* Item)
{
	if (auto* InventoryItem = Cast<FInventoryItemData>(Item))
		return InventoryItem;
	return nullptr;
}
