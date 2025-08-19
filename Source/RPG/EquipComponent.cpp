// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipComponent.h"
#include "EquipableItem.h"

// Sets default values for this component's properties
UEquipComponent::UEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UEquipComponent::SetIsOpen(bool bOpen)
{
	bIsOpen = bOpen;
}

AEquipableItem* UEquipComponent::GetEquippedItem(EEquipSlotType Slot) const
{
	return EquippedItems.Contains(Slot) ? EquippedItems[Slot] : nullptr;
}