// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EquipSlotType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EEquipSlotType : uint8
{
	RightHand UMETA(DisplayName = "RightHand"),
	LeftHand UMETA(DisplayName = "LeftHand"),
	Head UMETA(DisplayName = "Head"),
	Chest UMETA(DisplayName = "Chest"),
	Glove UMETA(DisplayName = "Glove"),
	Leg UMETA(DisplayName = "Leg"),
	QuickSlot UMETA(DisplayName = "QuickSlot")
	//향후 더 추가 예정
};
