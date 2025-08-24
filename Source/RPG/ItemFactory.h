// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EquipableItemData.h"
#include "NonEquipableItemData.h"
#include "ItemFactory.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UItemFactory : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void AutoRegisterItemClasses(); //액터 클래스 자동 매핑

	class AItemBase* SpawnItemFromTableID(UWorld* World, int32 ItemTableID);

	void InitializeItem(class AItemBase* Item, const FItemData* BaseData);
	void InitializeItem(class AEquipableItem* Item, const FEquipableItemData* BaseData);
	void InitializeItem(class ANonEquipableItem* Item, const FNonEquipableItemData* BaseData);

	FItemData* FindItemData(int32 ItemTableID);

private:
	TMap<int32, TSubclassOf<class AItemBase>> ItemClassMap;
};
