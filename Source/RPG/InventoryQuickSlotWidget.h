// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryQuickSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UInventoryQuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();


	void SetQuickSlotThumbnail(class UTexture2D* NewQuickSlotItemIcon);

	UPROPERTY(meta = (BindWidget))
	class UImage* QuickSlotThumbnail;

	UDataTable* ItemTable;
};
