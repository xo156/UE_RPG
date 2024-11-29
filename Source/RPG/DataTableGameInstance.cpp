// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTableGameInstance.h"
#include "Engine/DataTable.h"
#include "Camera/CameraShakeBase.h"
#include "DropRate.h"
#include "ItemData.h"


void UDataTableGameInstance::LoadAllTables()
{
	LoadItemCache();
	LoadItemDropCache();
}

FDropRate* UDataTableGameInstance::GetDropRate(int32 ItemID)
{
	FDropRate** FoundItem = ItemDropCache.Find(ItemID);
	return FoundItem ? *FoundItem : nullptr;
}

UDataTable* UDataTableGameInstance::GetItemTable()
{
	return ItemTable ? ItemTable : nullptr;
}

UDataTable* UDataTableGameInstance::GetDropItemTable()
{
	return DropItemTable ? DropItemTable : nullptr;
}

UDataTable* UDataTableGameInstance::GetMonsterDataTable()
{
	return MonsterDataTable ? MonsterDataTable : nullptr;
}

TSubclassOf<class UCameraShakeBase> UDataTableGameInstance::GetCameraShake()
{
	return CameraShake ? CameraShake : nullptr;
}

TSubclassOf<class UCameraShakeBase> UDataTableGameInstance::GetBossCameraShake()
{
	return BossCameraShake ? BossCameraShake : nullptr;
}

void UDataTableGameInstance::LoadItemCache()
{
	if (ItemTable) {
		static const FString ContextString(TEXT("Item Cache Context"));
		TArray<FItemData*> ItemRows;
		ItemTable->GetAllRows<FItemData>(ContextString, ItemRows);
		for (FItemData* Item : ItemRows) {
			ItemCache.Add(Item->ItemID, Item);
		}
	}
}

void UDataTableGameInstance::LoadItemDropCache()
{
	if (DropItemTable) {
		static const FString ContextString(TEXT("Item Drop Cache Context"));
		TArray<FDropRate*> ItemRows;
		DropItemTable->GetAllRows<FDropRate>(ContextString, ItemRows);
		for (FDropRate* Item : ItemRows) {
			ItemDropCache.Add(Item->ItemID, Item);
		}
	}
}
