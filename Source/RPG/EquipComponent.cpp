// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipComponent.h"
#include "EquipableItem.h"
#include "MyCharacter.h"
#include "InventoryComponent.h"
#include "ItemFactory.h"
#include "Kismet/GameplayStatics.h"
#include "DataTableGameInstance.h"

// Sets default values for this component's properties
UEquipComponent::UEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UEquipComponent::InitializeEquipSlots(const TArray<EEquipSlotType>& SlotTypes)
{
    EquipSlots.Empty();
    for (EEquipSlotType SlotType : SlotTypes) {
		if (SlotType == EEquipSlotType::QuickSlot)
			continue; //�� ������ ���⿡���� ���� �ʴ´�
        FEquipSlot NewSlot;
        NewSlot.SlotType = SlotType;
        EquipSlots.Add(NewSlot);
    }
}

bool UEquipComponent::EquipItem(const FInventoryItemData& ItemData, EEquipSlotType SlotType)
{
	if (ItemData.ItemTableID == 0) 
		return false;

	FEquipSlot* EquipSlot = GetSlot(SlotType);
	if (!EquipSlot) 
		return false;

	//���� �����ϰ� ������
	if (EquipSlot->EquippedItem.ItemTableID != 0) {
		UnequipItem(SlotType);
	}

	//���Կ� �� ������ ����
	EquipSlot->EquippedItem = ItemData;

	//ĳ���� �ݿ�
	ApplyEquipToCharacter(*EquipSlot, ItemData);

	return true;
}

bool UEquipComponent::UnequipItem(EEquipSlotType SlotType)
{
	FEquipSlot* EquipSlot = GetSlot(SlotType);
	if (!EquipSlot) 
		return false;

	//�����ϰ� �ִ°� ������
	if (EquipSlot->EquippedItem.ItemTableID == 0)
		return false;

	//ĳ���Ϳ��� ����
	RemoveEquipFromCharacter(*EquipSlot);

	//���� ���
	EquipSlot->EquippedItem = FInventoryItemData();
	EquipSlot->EquipedActor = nullptr;

	return true;
}

FEquipSlot* UEquipComponent::GetSlot(EEquipSlotType SlotType)
{
    //��ġ�ϴ� ù ��° ������ ����
    return EquipSlots.FindByPredicate([SlotType](const FEquipSlot& EquipSlot) {
        return EquipSlot.SlotType == SlotType;
    });
}

FName UEquipComponent::GetSocketNameBySlot(const FEquipSlot& EquipSlot) const
{
	switch (EquipSlot.SlotType)
	{
	case EEquipSlotType::None:
		return NAME_None;
		break;
	case EEquipSlotType::RightHand:
		return TEXT("HandSocket_R");
		break;
	case EEquipSlotType::LeftHand:
		return TEXT("HandSocket_L");
		break;
	case EEquipSlotType::Head:
		return TEXT("HeadSocket");
		break;
	case EEquipSlotType::Body:
		return TEXT("BodySocket");
		break;
	case EEquipSlotType::Arm:
		return TEXT("ArmSocket");
		break;
	case EEquipSlotType::Leg:
		return TEXT("LegSocket");
		break;
	default:
		return NAME_None;
		break;
	}

	return FName();
}

void UEquipComponent::ApplyEquipToCharacter(FEquipSlot& EquipSlot, const FInventoryItemData& ItemData)
{
	auto* ItemFactory = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UItemFactory>();
	if (!ItemFactory)
		return;

	auto* OwnerCharacter = Cast<AMyCharacter>(GetOwner());
	if (!OwnerCharacter && !OwnerCharacter->GetMesh())
		return;

	FName EquipSocketName = GetSocketNameBySlot(EquipSlot);

	auto* SpawnItem = ItemFactory->SpawnItemFromTableID(GetWorld(), ItemData.ItemTableID);
	if (!SpawnItem)
		return;

	auto* EquipItem = Cast<AEquipableItem>(SpawnItem);
	if (EquipItem)
		EquipItem->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, EquipSocketName);

	EquipSlot.EquipedActor = EquipItem;
}

void UEquipComponent::RemoveEquipFromCharacter(FEquipSlot& EquipSlot)
{
	if (IsValid(EquipSlot.EquipedActor)) {
		EquipSlot.EquipedActor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		EquipSlot.EquipedActor->Destroy();
	}
	EquipSlot.EquipedActor = nullptr;
}

void UEquipComponent::SetIsOpen(bool bOpen)
{
	bIsOpen = bOpen;
}
