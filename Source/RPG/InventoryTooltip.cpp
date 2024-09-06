// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTooltip.h"
#include "Components/TextBlock.h"
#include "ItemData.h"

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	ItemName->SetText(ItemData.ItemName);
	ItemValue->SetText(FText::Format(FText::FromString(TEXT("���ݷ�: {0}")), FText::AsNumber(ItemData.ItemAttackValue)));
	ItemDescription->SetText(FText::FromString(ItemData.ItemDescription));
}
