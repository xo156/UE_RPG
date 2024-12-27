// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMyGameModeBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	const TMap<int32, struct FDropRate*>& GetItemDropCache() { return ItemDropCache; }
	const TMap<int32, struct FItemData*>& GetItemCache() { return ItemCache; }
	const TMap<int32, struct FMonsterData*>& GetMonsterDataCache() { return MonsterDataCache; }
	const TMap<int32, struct FCharacterData*>& GetCharacterDataCache() { return CharacterDataCache; }

private:
	TMap<int32, struct FDropRate*> ItemDropCache;
	TMap<int32, struct FItemData*> ItemCache;
	TMap<int32, struct FMonsterData*> MonsterDataCache;
	TMap<int32, struct FCharacterData*> CharacterDataCache;
};
