// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipableItem.h"
#include "EquipSlotType.h"

// Sets default values
AEquipableItem::AEquipableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AEquipableItem::InitEquipableData(const FEquipableItemData* InItemData)
{
	if (InItemData)
		EquipableItemData = InItemData;
}

void AEquipableItem::SetOwnerCharacter(ACharacter* NewOwner)
{
	if (NewOwner)
		OwnerCharacter = NewOwner;
	
}

void AEquipableItem::EquipToCharacter(USkeletalMeshComponent* TargetMesh, FName SocketName)
{
	if (!TargetMesh)
		return;

	AttachToComponent(TargetMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	AttachedSocketName = SocketName;
}

void AEquipableItem::UnEquip()
{
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	AttachedSocketName = NAME_None;
}