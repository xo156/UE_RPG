// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "EquipSlotType.h"
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

	//장착/해제는 UEquipComponent 호출할 때 사용
	void AttachToSocket(USkeletalMeshComponent* TargetMesh, FName SocketName);
	void DetachFromSocket();

	int32 GetAttackPower() const { return EquipableItemData->AttackPower; }
	int32 GetDefensePower() const { return EquipableItemData->DefensePower; }
	
protected:
	ACharacter* OwnerCharacter = nullptr;
	FName AttachedSocketName = NAME_None;
	const FEquipableItemData* EquipableItemData;
};
