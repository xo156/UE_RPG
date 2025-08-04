// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemFactory.h"
#include "ItemData.h"
#include "DataTableGameInstance.h"
#include "Engine/World.h"
#include "ItemBase.h"
#include "EngineUtils.h"

void UItemFactory::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    AutoRegisterItemClasses();
}

void UItemFactory::AutoRegisterItemClasses()
{
    ItemClassMap.Empty();

	if (const UDataTableGameInstance* GameInstance = Cast<UDataTableGameInstance>(GetGameInstance())) {
		const TMap<int32, FItemData*>& ItemDataMap = GameInstance->GetItemCache();

		for (const auto& Pair : ItemDataMap) {
			const int32 ItemID = Pair.Key;
			const FItemData* ItemData = Pair.Value;

			if (ItemData) {
				if (UClass* LoadedClass = ItemData->ItemClassPath.TryLoadClass<AItemBase>()) {
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
	if (TSubclassOf<AItemBase>* FoundClass = ItemClassMap.Find(ItemTableID)) {
		if (World && *FoundClass) {
			auto* SpawnedItem = World->SpawnActor<AItemBase>(*FoundClass);
			if (SpawnedItem) {
				//데이터 캐시에서 정보 세팅
				if (UDataTableGameInstance* GameInstance = Cast<UDataTableGameInstance>(GetGameInstance())) {
					if (FItemData* FoundData = GameInstance->FindItemData(ItemTableID)) {
						SpawnedItem->SetItemData(FoundData);
					}
					else {
						UE_LOG(LogTemp, Warning, TEXT("UItemFactory: ItemData not found for ID: %d"), ItemTableID);
					}
				}
			}
			return SpawnedItem;
		}
	}
	return nullptr;
}
