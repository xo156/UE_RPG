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
	//Ä³½Ã
	void LoadAllTables();

	//getter
	struct FDropRate* GetDropRate(int32 ItemID);
	class UDataTable* GetItemTable();
	class UDataTable* GetDropItemTable();
	class UDataTable* GetMonsterDataTable();
	TSubclassOf<class UCameraShakeBase> GetCameraShake();
	TSubclassOf<class UCameraShakeBase> GetBossCameraShake();

	const TMap<int32, struct FDropRate*>& GetItemDropCache() const { return ItemDropCache; }
	const TMap<int32, struct FItemData*>& GetItemCache() const { return ItemCache; }

private:
	void LoadItemCache();
	void LoadItemDropCache();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* ItemTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* DropItemTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* MonsterDataTable;

	TMap<int32, struct FDropRate*> ItemDropCache;
	TMap<int32, struct FItemData*> ItemCache;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCameraShakeBase> BossCameraShake;
};
