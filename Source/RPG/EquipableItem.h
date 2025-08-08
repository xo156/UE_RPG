// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "EquipableItemData.h"
#include "EquipableItem.generated.h"

UCLASS(Abstract)
class RPG_API AEquipableItem : public AItemBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipableItem();

	void InitEquipableData(const FEquipableItemData* InItemData);

	void SetOwnerCharacter(ACharacter* NewOwner);

	void EquipToCharacter(USkeletalMeshComponent* TargetMesh, FName SocketName);
	void UnEquip();

	int32 GetAttackPower() const { return EquipableItemData->AttackPower; }
	int32 GetDefensePower() const { return EquipableItemData->DefensePower; }
	//EEquipSlot GetEquipSlot() const { return EquipableItemData->EquipSlot; }

protected:
	ACharacter* OwnerCharacter;

	FName AttachedSocketName;

	const FEquipableItemData* EquipableItemData;
};
