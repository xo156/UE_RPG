// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemData.h"
#include "InventorySlotWidget.generated.h"

UCLASS()
class RPG_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnThumbnailHovered();
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UFUNCTION()
	void OnUnThumbnailHovered();
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void OnThumbnailClicked();
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void RefreshSlot(TArray<FInventoryItemData> InventoryItem, int32 SlotIndex);
	void ClearSlot();

	void SetParentInventoryWidget(class UInventoryWidget* InWidget);
	void SetIsFocused(bool bFocused);
	bool IsFocused() const { return bIsFocused; }

	FInventoryItemData GetCurrentInventoryItemData() const { return CurrentInventoryItemData ; }

protected:
	UPROPERTY(meta = (BindWidget))
	class UBorder* FocusBorder;

	UPROPERTY(meta = (BindWidget))
	class UButton* SlotThumbnail;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmountText;

private:
	FInventoryItemData CurrentInventoryItemData;
	TMap<int32, FItemData*> ItemCache;
	bool bIsFocused = false;
	UInventoryWidget* ParentInventoryWidget = nullptr;
};
