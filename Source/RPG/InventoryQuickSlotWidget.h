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

	void SetQuickSlotConsumable(class UTexture2D* NewQuickSlotItemIcon, int32 InventoryItemAmount);
	void UpdateQuickSlotItemAmount(int32 NewAmount);


	UPROPERTY(meta = (BindWidget))
	class UOverlay* QuickSlotComsuable;

	UPROPERTY(meta = (BindWidget))
	class UImage* QuickSlotImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuickSlotAmount;

	class AItemBase* QuickSlotItem;
};
