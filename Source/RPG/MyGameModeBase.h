// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DropRate.h"
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

	const TMap<int32, FDropRate*>& GetItemDropCache() { return ItemDropCache; }

private:
	TMap<int32, FDropRate*> ItemDropCache;
};
