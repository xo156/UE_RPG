// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
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

	void AutoRegisterItemClasses(); //���� Ŭ���� �ڵ� ����

	class AItemBase* SpawnItemFromTableID(UWorld* World, int32 ItemTableID);

private:
	TMap<int32, TSubclassOf<class AItemBase>> ItemClassMap;
};
