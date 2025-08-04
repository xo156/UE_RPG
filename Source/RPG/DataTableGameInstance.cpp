// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTableGameInstance.h"
#include "Engine/DataTable.h"
#include "ItemFactory.h"
#include "InventoryItemData.h"

void UDataTableGameInstance::Init()
{
	Super::Init();
	LoadAllTableAndCache();

}

void UDataTableGameInstance::LoadAllTableAndCache()
{
	LoadItemCache();
	LoadItemDropCache();
	LoadMonsterDataCache();
}

FDropRate* UDataTableGameInstance::GetDropRate(int32 ItemTableID)
{
	FDropRate** FoundItem = ItemDropCache.Find(ItemTableID);
	return FoundItem ? *FoundItem : nullptr;
}

FMonsterData* UDataTableGameInstance::GetMonsterInfo(int32 MonsterID)
{
	FMonsterData** FoundMonster = MonsterDataCache.Find(MonsterID);
	return FoundMonster ? *FoundMonster : nullptr;
}

UDataTable* UDataTableGameInstance::GetItemTable()const 
{
	return ItemTable ? ItemTable : nullptr;
}

UDataTable* UDataTableGameInstance::GetDropItemTable()const 
{
	return DropItemTable ? DropItemTable : nullptr;
}

UDataTable* UDataTableGameInstance::GetMonsterDataTable() const
{
	return MonsterDataTable ? MonsterDataTable : nullptr;
}

FItemData* UDataTableGameInstance::FindItemData(int32 ItemTableID)
{
	if (FItemData** FoundData = ItemCache.Find(ItemTableID)) {
		return *FoundData;
	}

	UE_LOG(LogTemp, Warning, TEXT("FindItemData: ItemTableID %d not found in ItemCache."), ItemTableID);
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
}

void UDataTableGameInstance::LoadItemDropCache()
{
	if (DropItemTable) {
		static const FString ContextString(TEXT("Item Drop Cache Context"));
		TArray<FDropRate*> ItemDropRows;
		DropItemTable->GetAllRows<FDropRate>(ContextString, ItemDropRows);
		for (FDropRate* ItemDrop : ItemDropRows) {
			ItemDropCache.Add(ItemDrop->ItemTableID, ItemDrop);
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
