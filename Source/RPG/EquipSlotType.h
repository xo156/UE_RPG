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
	None UMETA(DisplayName = "None"),
	RightHand UMETA(DisplayName = "RightHand"),
	LeftHand UMETA(DisplayName = "LeftHand"),
	Head UMETA(DisplayName = "Head"),
	Body UMETA(DisplayName = "Body"),
	Arm UMETA(DisplayName = "Arm"),
	Leg UMETA(DisplayName = "Leg"),
	QuickSlot UMETA(DisplayName = "QuickSlot")
};
