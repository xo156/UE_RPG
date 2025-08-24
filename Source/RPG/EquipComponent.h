// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipSlotType.h"
#include "InventoryItemData.h"
#include "EquipComponent.generated.h"

USTRUCT(BlueprintType)
struct FEquipSlot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	EEquipSlotType SlotType;

	UPROPERTY(BlueprintReadOnly)
	FInventoryItemData EquippedItem;

	UPROPERTY(BlueprintReadOnly)
	class AEquipableItem* EquipedActor = nullptr;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipComponent();

	void InitializeEquipSlots(const TArray<EEquipSlotType>& SlotTypes);

	//ÀåÂø ¹× Å»Âø
	bool EquipItem(const FInventoryItemData& ItemData, EEquipSlotType SlotType);
	bool UnequipItem(EEquipSlotType SlotType);

	FEquipSlot* GetSlot(EEquipSlotType SlotType);

	//½ÇÁ¦ Ä³¸¯ÅÍ¿¡°Ô ÀåÂø
	FName GetSocketNameBySlot(const FEquipSlot& EquipSlot) const;
	void ApplyEquipToCharacter(FEquipSlot& EquipSlot, const FInventoryItemData& ItemData);
	void RemoveEquipFromCharacter(FEquipSlot& EquipSlot);

	void SetIsOpen(bool bOpen);
	bool IsOpen() { return bIsOpen; }

private:
	bool bIsOpen;

	TArray<FEquipSlot> EquipSlots;
};
