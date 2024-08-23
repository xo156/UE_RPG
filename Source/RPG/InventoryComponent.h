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

	int32 FindSlotIndex(class ADropItem* AddItem);
	void StackItem(class ADropItem* AddItem, int32 SlotIndex);
	void AddItem(class ADropItem* AddItem);
	int32 MakeUID();

	void CreateInventoryWidget();
	void OpenInventoryWidget();
	void CloseInventoryWidget();

	FInventoryItemData InventoryItemData;
	TArray<FInventoryItemData> Inventory;

	int32 MaxSlotCounter = 20;
	int32 CurrentSlotCounter = 0;
	int32 UIDCounter = 0;

	bool bIsOpen = false;
};
