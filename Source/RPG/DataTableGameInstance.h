// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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
	struct FDropRate* GetDropRate(int32 ItemID);
	//struct FMonsterData* GetMonsterInfo(int32 MonsterID);
	struct FCharacterData* GetCharacterInfo(int32 CharacterID);
	class UDataTable* GetItemTable();
	class UDataTable* GetDropItemTable();
	class UDataTable* GetMonsterDataTable();
	class UDataTable* GetCharacterDataTable();
	TSubclassOf<class UCameraShakeBase> GetCameraShake();
	TSubclassOf<class UCameraShakeBase> GetBossCameraShake();

	const TMap<int32, struct FDropRate*>& GetItemDropCache() const { return ItemDropCache; }
	const TMap<int32, struct FItemData*>& GetItemCache() const { return ItemCache; }
	const TMap<int32, struct FMonsterData*>& GetMonsterDataCache() const { return MonsterDataCache; }
	const TMap<int32, struct FCharacterData*>& GetCharacterDataCache() const { return CharacterDataCache; }

private:
	void LoadItemCache();
	void LoadItemDropCache();
	void LoadMonsterDataCache();
	void LoadCharacterDataCache();

	//테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* ItemTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* DropItemTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* MonsterDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* CharacterDataTable;

	//캐시
	TMap<int32, struct FDropRate*> ItemDropCache;
	TMap<int32, struct FItemData*> ItemCache;
	TMap<int32, struct FMonsterData*> MonsterDataCache;
	TMap<int32, struct FCharacterData*> CharacterDataCache;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCameraShakeBase> BossCameraShake;
};
