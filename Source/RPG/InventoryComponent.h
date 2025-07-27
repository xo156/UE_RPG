// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItemData.h"
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

	bool TryAddItem(class ADropItem* AddedItem);
	int32 FindSlotIndex(class ADropItem* AddedItem);
	void StackItem(class ADropItem* AddedItem, int32 SlotIndex);
	void AddItem(class ADropItem* AddedItem);
	void RemoveItem(int32 ItemTableID, int32 Amount);
	int32 MakeUID();
	int32 GetInventoryItemAmount(int32 FindItemID);

	bool IsOpen() const { return bIsOpen; }
	void SetIsOpen(bool bOpen);

	TArray<FInventoryItemData> InventoryItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UInventoryWidget> InventoryWidgetClass;
	class UInventoryWidget* InventoryWidget;

	int32 MaxSlotCounter;
	int32 CurrentSlotCounter;
	int32 UIDCounter;

	bool bIsOpen;
};
