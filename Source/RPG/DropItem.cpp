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
	DropItemCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = DropItemCollision;

	DropItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	DropItemMesh->SetCollisionProfileName(TEXT("NoCollision"));
	DropItemMesh->SetVisibility(true);
	DropItemMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADropItem::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("DropItem BeginPlay: Location: %s"), *GetActorLocation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("DropItemMesh Visibility: %s"), DropItemMesh->IsVisible() ? TEXT("Visible") : TEXT("Not Visible"));

	UE_LOG(LogTemp, Warning, TEXT("ADropItem BeginPlay: Location: %s"), *GetActorLocation().ToString());

}

// Called every frame
void ADropItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(50.f), FColor::Red, false, -1.f, 0, 2.f);

}

void ADropItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogTemp, Warning, TEXT("ADropItem EndPlay: Reason: %d"), (int32)EndPlayReason);

}

void ADropItem::SetDropItem(const FDropItemData& NewDropItemData)
{
	DropItemData = NewDropItemData;

	UE_LOG(LogTemp, Warning, TEXT("Drop DropItem ID: %d, Amount: %d, Counterble: %s"),
		   DropItemData.ItemID,
		   DropItemData.Amount,
		   DropItemData.bCounterble ? TEXT("True") : TEXT("False"));
}
