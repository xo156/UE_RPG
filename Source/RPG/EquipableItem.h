// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquipableItem.generated.h"

UCLASS()
class RPG_API AEquipableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipableItem();

	void SetOwnerCharacter(ACharacter* NewOwner);

	void EquipToCharacter(USkeletalMeshComponent* TargetMesh, FName SocketName);
	void UnEquip();

protected:
	ACharacter* OwnerCharacter;

	FName AttachedSocketName;
};
