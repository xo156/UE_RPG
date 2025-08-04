// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropItemData.h"
#include "DropItem.generated.h"

UCLASS()
class RPG_API ADropItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropItem();

	void SetDropItemData(const TArray<FDropItemData>& NewDropItemDatas);
	TArray<FDropItemData> GetDropItemDatas() { return DropItemDatas; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UBoxComponent* DropItemCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UStaticMeshComponent* DropItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FDropItemData> DropItemDatas;
};
