// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

void AItemBase::InitItemData(const FItemData* InItemData)
{
	if (InItemData)
		ItemData = InItemData;
}
