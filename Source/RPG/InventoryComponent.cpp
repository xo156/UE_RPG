// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "DropItem.h"
#include "InventoryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "DataTableGameInstance.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

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

bool UInventoryComponent::TryAddItem( FDropItemData& AddedItem)
{
	int32 SlotIndex = FindSlotIndex(AddedItem.ItemTableID);
	if (SlotIndex != INDEX_NONE) {
		//이미 있는 아이템이면
		StackItem(AddedItem, SlotIndex);
	}
	else {
		if (CurrentSlotCounter >= MaxSlotCounter) {
			UE_LOG(LogTemp, Error, TEXT("Inventory Full"));
			return false;
		}
		else {
			//없는 아이템이면
			AddItem(AddedItem);
		}
	}
	return true;
}

int32 UInventoryComponent::FindSlotIndex(int32 ItemTableID)
{
	for (int32 i = 0; i < InventoryItems.Num(); i++) {
		if (InventoryItems[i].ItemTableID == ItemTableID) {
			return i;
		}
	}
	return INDEX_NONE;
}

void UInventoryComponent::StackItem(FDropItemData& AddedItem, int32 SlotIndex)
{
	auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance || !InventoryItems.IsValidIndex(SlotIndex))
		return;

	const FItemData* ItemData = GameInstance->FindItemData(AddedItem.ItemTableID);
	if (!ItemData)
		return;

	FInventoryItemData& SlotItemData = InventoryItems[SlotIndex];
	int32 MaxStackCount = ItemData->MaxStackCount;
	int32 AvailableStackCount = MaxStackCount - SlotItemData.ItemAmount;

	int32 StackableAmount = FMath::Min(AvailableStackCount, AddedItem.Amount);
	SlotItemData.ItemAmount += StackableAmount;
	if (AddedItem.Amount - StackableAmount > 0) {
		if (CurrentSlotCounter < MaxSlotCounter) {
			AddedItem.Amount -= StackableAmount;
			AddItem(AddedItem);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Inventory Full"));
		}
	}
}

void UInventoryComponent::AddItem(const FDropItemData& AddedItem)
{
	FInventoryItemData NewInventoryItemData;
	NewInventoryItemData.ItemUID = MakeUID();
	NewInventoryItemData.ItemAmount = AddedItem.Amount;
	NewInventoryItemData.ItemTableID = AddedItem.ItemTableID;

	InventoryItems.Add(NewInventoryItemData);
	CurrentSlotCounter++;
}

void UInventoryComponent::RemoveItem(int32 ItemTableID, int32 Amount)
{
	for (int32 Index = 0; Index < InventoryItems.Num(); Index++) {
		if (InventoryItems[Index].ItemTableID == ItemTableID) {
			InventoryItems[Index].ItemAmount = FMath::Max(InventoryItems[Index].ItemAmount - Amount, 0);
			if (InventoryItems[Index].ItemAmount <= 0) {
				InventoryItems.RemoveAt(Index);
			}
			break;
		}
	}
}

int32 UInventoryComponent::MakeUID()
{
	return ++UIDCounter;
}

int32 UInventoryComponent::GetInventoryItemAmount(int32 FindItemID)
{
	for (auto& InventoryItem : InventoryItems) {
		if (InventoryItem.ItemTableID == FindItemID)
			return InventoryItem.ItemAmount;
	}
	return 0;
}

void UInventoryComponent::OverFlowAddedItem(const FDropItemData& OverFlowItem)
{
	UE_LOG(LogTemp, Warning, TEXT("Inventory full. Dropping remaining item (ID: %d, Amount: %d)"),
		   OverFlowItem.ItemTableID, OverFlowItem.Amount);

	//남은거는 필드에 아이템 드랍 처리
	FVector DropLocation = GetOwner()->GetActorLocation();
	FActorSpawnParameters Params;
	auto* DropActor = GetWorld()->SpawnActor<ADropItem>(ADropItem::StaticClass(), DropLocation, FRotator::ZeroRotator, Params);
	if (DropActor) {
		TArray<FDropItemData> DropItems;
		DropItems.Add(OverFlowItem);
		DropActor->SetDropItemData(DropItems);
	}
}

void UInventoryComponent::SetIsOpen(bool bOpen)
{
	bIsOpen = bOpen;
}
