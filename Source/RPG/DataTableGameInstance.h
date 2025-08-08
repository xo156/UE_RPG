// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DropRate.h"
#include "MonsterData.h"
#include "ItemData.h"
#include "EquipableItemData.h"
#include "NonEquipableItemData.h"
#include "DataTableGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UDataTableGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	//캐시
	void LoadAllTableAndCache();

	//getter
	FMonsterData* GetMonsterInfo(int32 MonsterID);
	FItemData* FindItemData(int32 ItemTableID);
	FEquipableItemData* FindEquipableItemData(int32 ItemID);
	FNonEquipableItemData* FindNonEquipableItemData(int32 ItemID);

	const TMap<int32, FItemData*>& GetItemCache() const { return ItemCache; }
	const TMap<int32, FMonsterData*>& GetMonsterDataCache() const { return MonsterDataCache; }
	
private:
	void LoadItemCache();
	void LoadMonsterDataCache();

	//테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* ItemTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* MonsterDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* EquipableItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* NonEquipableItemDataTable;

	//캐시
	TMap<int32, FItemData*> ItemCache;
	TMap<int32, FMonsterData*> MonsterDataCache;
	TMap<int32, FEquipableItemData*> EquipableItemCache;
	TMap<int32, FNonEquipableItemData*> NonEquipableItemCache;
};
