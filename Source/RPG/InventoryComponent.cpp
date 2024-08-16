// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemBase.h"
#include "MyCharacter.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// ...
}

void UInventoryComponent::AddItem(const FInventoryItemData& NewItem)
{
    
}

void UInventoryComponent::UseItem(int32 ItemUID, int32 Quantity)
{
	if (ItemDataTable == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("ItemDataTable is Null"));
		return;
	}

    static const FString ContextString(TEXT("Item Data Context"));

	for (FInventoryItemData& InventoryItem : Inventory) {
		if (InventoryItem.ItemUID == ItemUID) {
			// 아이템 데이터 테이블에서 아이템 정보를 가져옴
			FItemStruct* ItemData = ItemDataTable->FindRow<FItemStruct>(FName(*FString::FromInt(InventoryItem.ItemTableID)), ContextString);

			if (ItemData && ItemData->ItemClass) {
				// 아이템 클래스가 정의되어 있는 경우
				AItemBase* ItemActor = GetWorld()->SpawnActor<AItemBase>(ItemData->ItemClass, GetOwner()->GetActorLocation(), FRotator::ZeroRotator);
				if (ItemActor) {
					ItemActor->Use();  // Use() 함수 호출
					InventoryItem.ItemCurrentAmount -= Quantity;
					if (InventoryItem.ItemCurrentAmount <= 0) {
						RemoveItem(ItemUID, InventoryItem.ItemCurrentAmount);
					}
				}
			}
			break;
		}
	}
}

void UInventoryComponent::RemoveItem(int32 ItemUID, int32 Quantity)
{
	/*for (FInventoryItemData& InventoryItem : Inventory) {
		if (InventoryItem.ItemUID == ItemUID) {
			if (Quantity >= InventoryItem.ItemCurrentAmount) {
				Inventory.Remove(InventoryItem);
			}
			else {
				InventoryItem.ItemCurrentAmount -= Quantity;
			}
			break;
		}
	}*/
}
