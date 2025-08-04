// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NonEquipableItem.generated.h"

UCLASS(Abstract)
class RPG_API ANonEquipableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANonEquipableItem();

	virtual void UseItem();
};
