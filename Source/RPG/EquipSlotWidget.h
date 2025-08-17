// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FocusableSlotWidget.h"
#include "EquipSlotType.h"
#include "EquipSlotWidget.generated.h"


UCLASS()
class RPG_API UEquipSlotWidget : public UFocusableSlotWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void InitEquipSlot(int32 InIndex, EEquipSlotType InEquipSlotType, bool bInAllowWeapon);

	UFUNCTION()
	virtual void OnSlotConfirmed() override;

	void SetOwnerEquipWidget(class UEquipWidget* InOwner);

private:
	class UEquipWidget* OwnerEquipWidget;
	EEquipSlotType EquipSlotType;
	int32 SlotIndex;
	bool bAllowWeapon = false;
};
