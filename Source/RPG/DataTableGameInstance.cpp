// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTableGameInstance.h"
#include "Engine/DataTable.h"
#include "ItemFactory.h"
#include "InventoryItemData.h"

void UDataTableGameInstance::Init()
{
	Super::Init();
	LoadAllTableAndCache();

	if (auto* ItemFactory = GetSubsystem<UItemFactory>()) {
		ItemFactory->AutoRegisterItemClasses();
	}
}

void UDataTableGameInstance::LoadAllTableAndCache()
{
	LoadItemCache();
	LoadMonsterDataCache();
}

FMonsterData* UDataTableGameInstance::GetMonsterInfo(int32 MonsterID)
{
	if (FMonsterData** FoundMonster = MonsterDataCache.Find(MonsterID)) {
		return *FoundMonster;
	}
	 return nullptr;
}

FItemData* UDataTableGameInstance::FindItemData(int32 ItemTableID)
{
	if (FItemData** FoundData = ItemCache.Find(ItemTableID)) {
		return *FoundData;
	}
	return nullptr;
}

FEquipableItemData* UDataTableGameInstance::FindEquipableItemData(int32 ItemID)
{
	if (FEquipableItemData** FoundData = EquipableItemCache.Find(ItemID)) {
		return *FoundData;
	}
	return nullptr;
}

FNonEquipableItemData* UDataTableGameInstance::FindNonEquipableItemData(int32 ItemID)
{
	if (FNonEquipableItemData** FoundData = NonEquipableItemCache.Find(ItemID)) {
		return *FoundData;
	}
	return nullptr;
}

void UDataTableGameInstance::LoadItemCache()
{
	if (ItemTable) {
		static const FString ContextString(TEXT("Item Cache Context"));
		TArray<FItemData*> ItemRows;
		ItemTable->GetAllRows<FItemData>(ContextString, ItemRows);
		for (FItemData* Item : ItemRows) {
			ItemCache.Add(Item->ItemTableID, Item);
		}
	}

	if (EquipableItemDataTable) {
		static const FString Context(TEXT("Equipable Cache Context"));
		TArray<FEquipableItemData*> EquipRows;
		EquipableItemDataTable->GetAllRows(Context, EquipRows);
		for (FEquipableItemData* Data : EquipRows) {
			EquipableItemCache.Add(Data->ItemTableID, Data);
		}
	}

	if (NonEquipableItemDataTable) {
		static const FString Context(TEXT("NonEquipable Cache Context"));
		TArray<FNonEquipableItemData*> NonEquipRows;
		NonEquipableItemDataTable->GetAllRows(Context, NonEquipRows);
		for (FNonEquipableItemData* Data : NonEquipRows) {
			NonEquipableItemCache.Add(Data->ItemTableID, Data);
		}
	}
}

void UDataTableGameInstance::LoadMonsterDataCache()
{
	if (MonsterDataTable) {
		static const FString ContextString(TEXT("Monster Data Cache Context"));
		TArray<FMonsterData*> MonsterDataRows;
		MonsterDataTable->GetAllRows<FMonsterData>(ContextString, MonsterDataRows);
		for (FMonsterData* MonsterData : MonsterDataRows) {
			MonsterDataCache.Add(MonsterData->MonsterID, MonsterData);
		}
	}
}
