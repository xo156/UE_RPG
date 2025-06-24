// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTableGameInstance.h"
#include "Engine/DataTable.h"
#include "Camera/CameraShakeBase.h"
#include "DropRate.h"
#include "ItemData.h"
#include "InventoryItemData.h"
#include "MonsterData.h"
#include "CharacterData.h"

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
	LoadCharacterDataCache();
}

FDropRate* UDataTableGameInstance::GetDropRate(int32 ItemID)
{
	FDropRate** FoundItem = ItemDropCache.Find(ItemID);
	return FoundItem ? *FoundItem : nullptr;
}

//FMonsterData* UDataTableGameInstance::GetMonsterInfo(int32 MonsterID)
//{
//	FMonsterData** FoundMonster = MonsterDataCache.Find(MonsterID);
//	return FoundMonster ? *FoundMonster : nullptr;
//}

FCharacterData* UDataTableGameInstance::GetCharacterInfo(int32 CharacterID)
{
	FCharacterData** FoundCharacter = CharacterDataCache.Find(CharacterID);
	return FoundCharacter ? *FoundCharacter : nullptr;
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

UDataTable* UDataTableGameInstance::GetCharacterDataTable()
{
	return CharacterDataTable ? CharacterDataTable : nullptr;
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
		TArray<FDropRate*> ItemDropRows;
		DropItemTable->GetAllRows<FDropRate>(ContextString, ItemDropRows);
		for (FDropRate* ItemDrop : ItemDropRows) {
			ItemDropCache.Add(ItemDrop->ItemID, ItemDrop);
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

void UDataTableGameInstance::LoadCharacterDataCache()
{
	if (CharacterDataTable) {
		static const FString ContextString(TEXT("Character Data Cache Context"));
		TArray<FCharacterData*> CharacterDataRows;
		CharacterDataTable->GetAllRows<FCharacterData>(ContextString, CharacterDataRows);
		for (FCharacterData* CharacterData : CharacterDataRows) {
			CharacterDataCache.Add(CharacterData->PlayerCharacterID, CharacterData);
		}
	}
}
