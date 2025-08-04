// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DropRate.h"
#include "MonsterData.h"
#include "ItemData.h"
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
	FDropRate* GetDropRate(int32 ItemTableID);
	FMonsterData* GetMonsterInfo(int32 MonsterID);
	class UDataTable* GetItemTable() const;
	class UDataTable* GetDropItemTable() const;
	class UDataTable* GetMonsterDataTable() const;
	FItemData* FindItemData(int32 ItemTableID);

	const TMap<int32, FDropRate*>& GetItemDropCache() const { return ItemDropCache; }
	const TMap<int32, FItemData*>& GetItemCache() const { return ItemCache; }
	const TMap<int32, FMonsterData*>& GetMonsterDataCache() const { return MonsterDataCache; }

private:
	void LoadItemCache();
	void LoadItemDropCache();
	void LoadMonsterDataCache();

	//테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* ItemTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* DropItemTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* MonsterDataTable;

	//캐시
	TMap<int32, FDropRate*> ItemDropCache;
	TMap<int32, FItemData*> ItemCache;
	TMap<int32, FMonsterData*> MonsterDataCache;
};
