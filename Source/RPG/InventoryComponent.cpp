// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemBase.h"

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

void UInventoryComponent::AddItem(AItemBase* ItemBase)
{
	if (ItemBase) {
		InventoryItems.Add(ItemBase);
		UE_LOG(LogTemp, Log, TEXT("Item Add : %s"), *ItemBase->GetName());
	}
}

void UInventoryComponent::UseItem(AItemBase* ItemBase)
{
	if (ItemBase) {
		ItemBase->Use();
	}
}

void UInventoryComponent::DestroyItem(AItemBase* ItemBase)
{
	if (ItemBase) {
		InventoryItems.Remove(ItemBase);
		ItemBase->Destroy();
	}
}
