// Fill out your copyright notice in the Description page of Project Settings.


#include "NonEquipableItem.h"
#include "ConsumableItem.h"

// Sets default values
ANonEquipableItem::ANonEquipableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ANonEquipableItem::InitNonEquipableData(const FNonEquipableItemData* InItemData)
{
    if(InItemData)
        NonEquipableItemData = InItemData;
}

void ANonEquipableItem::UseItem()
{

}