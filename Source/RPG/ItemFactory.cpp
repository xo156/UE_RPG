// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemFactory.h"
#include "ItemData.h"
#include "ItemBase.h"
#include "EquipableItem.h"
#include "NonEquipableItem.h"
#include "Weapon.h"
#include "ConsumableItem.h"
#include "DataTableGameInstance.h"
#include "Engine/World.h"

void UItemFactory::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

}

void UItemFactory::AutoRegisterItemClasses()
{
    ItemClassMap.Empty();

	if (const UDataTableGameInstance* GameInstance = Cast<UDataTableGameInstance>(GetGameInstance())) {
		const TMap<int32, FItemData*>& ItemDataMap = GameInstance->GetItemCache();

		for (const auto& Pair : ItemDataMap) {
			const int32 ItemID = Pair.Key;
			const FItemData* ItemData = Pair.Value;

			if (ItemData && ItemData->ItemClassPath.IsValid()) {
				if (auto* LoadedClass = ItemData->ItemClassPath.TryLoadClass<AItemBase>()) {
					ItemClassMap.Add(ItemID, LoadedClass);
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("ItemFactory: Failed to load class for ItemID %d"), ItemID);
				}
			}
		}
	}
}

AItemBase* UItemFactory::SpawnItemFromTableID(UWorld* World, int32 ItemTableID)
{
	if (!World)
		return nullptr;

	TSubclassOf<AItemBase>* FoundClass = ItemClassMap.Find(ItemTableID);
	if (!FoundClass || !(*FoundClass))
		return nullptr;

	auto* SpawnedItem = World->SpawnActor<AItemBase>(*FoundClass);
	if (!SpawnedItem)
		return nullptr;

	auto* GameInstance = Cast<UDataTableGameInstance>(GetGameInstance());
	if (!GameInstance) {
		SpawnedItem->Destroy();
		return nullptr;
	}

	auto* FoundData = GameInstance->FindItemData(ItemTableID);
	if (!FoundData) {
		SpawnedItem->Destroy();
		return nullptr;
	}

	switch (FoundData->ItemType)
	{
	case EItemType::Weapon:
	case EItemType::Armor:
	{
		const auto* EquipData = GameInstance->FindEquipableItemData(ItemTableID);
		auto* EquipableItem = Cast<AEquipableItem>(SpawnedItem);
		if (EquipData && EquipableItem) {
			InitializeItem(EquipableItem, EquipData);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Failed to initialize EquipableItem for ID %d"), ItemTableID);
			SpawnedItem->Destroy();
			return nullptr;
		}
		break;

	}
	case EItemType::Consumable:
	case EItemType::Quest:
	{
		const auto* NonEquipData = GameInstance->FindNonEquipableItemData(ItemTableID);
		auto* NonEquipableItem = Cast<ANonEquipableItem>(SpawnedItem);
		if (NonEquipData && NonEquipableItem) {
			InitializeItem(NonEquipableItem, NonEquipData);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Failed to initialize NonEquipableItem for ID %d"), ItemTableID);
			SpawnedItem->Destroy();
			return nullptr;
		}
		break;
	}
	default:
		InitializeItem(SpawnedItem, FoundData);
		break;
	}
	return SpawnedItem;
}

void UItemFactory::InitializeItem(AItemBase* Item, const FItemData* BaseData)
{
	if (!Item || !BaseData)
		return;

	Item->InitItemData(BaseData);
}

void UItemFactory::InitializeItem(AEquipableItem* Item, const FEquipableItemData* BaseData)
{
	if (!Item || !BaseData)
		return;

	Item->InitItemData(BaseData);
	Item->InitEquipableData(BaseData);
}

void UItemFactory::InitializeItem(ANonEquipableItem* Item, const FNonEquipableItemData* BaseData)
{
	if (!Item || !BaseData)
		return;

	Item->InitItemData(BaseData);
	Item->InitNonEquipableData(BaseData);
}

FItemData* UItemFactory:: FindItemData(int32 ItemTableID)
{
	auto* GameInstance = Cast<UDataTableGameInstance>(GetGameInstance());
	if (!GameInstance)
		return nullptr;

	const TMap<int32, FItemData*>& ItemCache = GameInstance->GetItemCache(); // 참조로 받음
	if (FItemData* const* FoundData = ItemCache.Find(ItemTableID)) {
		return *FoundData;
	}
	return nullptr;
}