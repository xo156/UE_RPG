// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTooltip.h"
#include "Components/TextBlock.h"
#include "ItemData.h"

void UInventoryTooltip::PreConstruct(bool bIsDesignTime)
{
	Super::PreConstruct(bIsDesignTime);

	if (ItemName) {
		ItemName->SetText(FText::FromString("Item Name: (Default)"));
	}

	if (ItemValue) {
		ItemValue->SetText(FText::FromString("Item Value: 0"));
	}

	if (ItemDescription) {
		ItemDescription->SetText(FText::FromString("Item Description: (Default)"));
	}

	UE_LOG(LogTemp, Log, TEXT("UInventoryTooltip::PreConstruct"));
}

void UInventoryTooltip::InitTooltip(const FItemData& InItemData)
{
	if (ItemName) {
		ItemName->SetText(FText::FromString(InItemData.ItemName.ToString()));
	}
	
	if (ItemValue) {
		switch (InItemData.ItemType) {
		case EItemType::Consumable:
			ItemValue->SetText(FText::Format(FText::FromString(TEXT("회복량: {0}")), FText::AsNumber(InItemData.ItemRecoverValue)));
			break;
		case EItemType::Weapon:
			ItemValue->SetText(FText::Format(FText::FromString(TEXT("공격력: {0}")), FText::AsNumber(InItemData.ItemAttackValue)));
		default:
			ItemValue->SetText(FText::FromString(TEXT("N/A")));
			break;
		}
	}

	if (ItemDescription) {
		ItemDescription->SetText(FText::FromString(InItemData.ItemDescription));
	}
}
