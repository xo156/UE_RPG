// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ADropItem::ADropItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DropItemCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemCollision"));
	RootComponent = DropItemCollision;
	DropItemCollision->SetCollisionProfileName(TEXT("OverlapAll"));

	DropItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	DropItemMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void ADropItem::BeginPlay()
{
	Super::BeginPlay();
	
	InitDropItems(ItemsToDrop);
}

// Called every frame
void ADropItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADropItem::CalcDropItems(TArray<int32> DropableItemIDS, UDataTable* ItemTable, TArray<FItemDrop>& OutItemsToDrop)
{
    static const FString ContextString(TEXT("Item Drop Context"));

    for (int32 DropItemID : DropableItemIDS) {
        FItemDrop* ItemRow = ItemTable->FindRow<FItemDrop>(FName(*FString::FromInt(DropItemID)), ContextString);

        if (ItemRow) {
            float RandomValue = FMath::FRandRange(0.0f, 100.0f);
            if (RandomValue <= ItemRow->ItemRate) {
                int32 DropAmount = FMath::RandRange(ItemRow->MinDropItemAmount, ItemRow->MaxDropItemAmount);

                for (int32 i = 0; i < DropAmount; ++i) {
                    OutItemsToDrop.Add(*ItemRow);
                }
            }
        }
    }
}

void ADropItem::InitDropItems(const TArray<FItemDrop>& Items)
{
	ItemsToDrop = Items;
}
