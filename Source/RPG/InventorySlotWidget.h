// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnThumbnailHovered();

	UFUNCTION()
	void OnThumbnailUnhovered();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void RefreshSlot(TArray<FInventoryItemData> InventoryItem, int32 SlotIndex);
	void ClearSlot();

	UPROPERTY(meta = (BindWidget))
	class UButton* Thumbnail;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmountText;

	FInventoryItemData CurrentInventoryItemData;
	TMap<int32, struct FItemData*> ItemCache;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class UInventoryTooltip> InventoryTooltipClass;
	class UInventoryTooltip* InventoryTooltipInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class UInventoryItemAction> InventoryItemActionClass;
	class UInventoryItemAction* InventoryItemActionInstance;

	bool bIsHover = false;
};
