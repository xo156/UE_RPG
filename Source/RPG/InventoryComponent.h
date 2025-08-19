// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItemData.h"
#include "DropItemData.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool TryAddItem(FDropItemData& AddedItem);
	int32 FindSlotIndex(int32 ItemTableID);
	void StackItem(FDropItemData& AddedItem, int32 SlotIndex);
	void AddItem(const FDropItemData& AddedItem);
	void RemoveItem(int32 ItemTableID, int32 Amount);
	int32 MakeUID();
	int32 GetInventoryItemAmount(int32 FindItemID);
	void OverFlowAddedItem(const FDropItemData& OverFlowItem);

	class UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
	int32 GetMaxSlotCounter() const { return MaxSlotCounter; }

	void SetIsOpen(bool bOpen);
	bool IsOpen() { return bIsOpen; }

	TArray<FInventoryItemData> GetInventoryItems() { return InventoryItems; }
	TArray<FInventoryItemData> GetFilteredInventoryItems(TArray<EItemType> Filters);
private:
	TArray<FInventoryItemData> InventoryItems;

	class UInventoryWidget* InventoryWidget;

	int32 MaxSlotCounter;
	int32 CurrentSlotCounter;
	int32 UIDCounter;

	bool bIsOpen;
};
