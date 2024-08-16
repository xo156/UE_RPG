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

	void AddItem(TArray<class ADropItem*> Items);
	void UseItem(class ADropItem* InventoryItem, int32 Quantity = 1);
	void RemoveItem(class ADropItem* InventoryItem, int32 Quantity = 1);

	int32 GetInventoryItemAmount(class ADropItem* Item);
	FInventoryItemData* GetInventoryItem(class ADropItem* Item);

	TArray<class ADropItem*> Inventory;
};
