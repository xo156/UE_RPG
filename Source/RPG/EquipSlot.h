// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EquipSlot.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EEquipSlot : uint8
{
	RightHand UMETA(DisplayName = "RightHand"),
	LeftHand UMETA(DisplayName = "LeftHand"),
	//향후 더 추가 예정
};
