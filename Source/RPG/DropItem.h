// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemDrop.h"
#include "DropItem.generated.h"

UCLASS()
class RPG_API ADropItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UBoxComponent* DropItemCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UStaticMeshComponent* DropItemMesh;

	//아이템 드랍
	void CalcDropItems(TArray<int32> DropableItemIDS, UDataTable* ItemTable, TArray<FItemDrop>& OutItemsToDrop);
	void InitDropItems(const TArray<FItemDrop>& Items);

	FItemDrop GetItemDrop();

private:
	TArray<FItemDrop> ItemsToDrop;
};
