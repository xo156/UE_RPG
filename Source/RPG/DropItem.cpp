// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ItemData.h"

// Sets default values
ADropItem::ADropItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DropItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	DropItemMesh->SetCollisionProfileName(TEXT("NoCollision"));
	DropItemMesh->SetVisibility(true);
	RootComponent = DropItemMesh;

	DropItemCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemCollision"));
	DropItemCollision->SetCollisionProfileName(TEXT("Root"));
	DropItemCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADropItem::BeginPlay()
{
	Super::BeginPlay();

	SetDropItemData(DropItemDatas);
}

void ADropItem::SetDropItemData(const TArray<FDropItemData>& NewDropItemDatas)
{
	DropItemDatas = NewDropItemDatas;
}