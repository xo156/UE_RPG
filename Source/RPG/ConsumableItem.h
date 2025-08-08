// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NonEquipableItem.h"
#include "ConsumableItem.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RPG_API AConsumableItem : public ANonEquipableItem
{
	GENERATED_BODY()

public:

	virtual void UseItem() override;

};
