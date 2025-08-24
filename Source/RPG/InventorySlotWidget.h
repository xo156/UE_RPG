// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FocusableSlotWidget.h"
#include "InventoryItemData.h"
#include "InventorySlotWidget.generated.h"

UCLASS()
class RPG_API UInventorySlotWidget : public UFocusableSlotWidget
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

	void RefreshSlot(FInventoryItemData InventoryItem);
	virtual void ClearSlot() override;

	void SetParentInventoryWidget(class UInventoryWidget* InWidget);
	void SetParentEquipWidget(class UEquipWidget* InWidget);

	FInventoryItemData GetCurrentInventoryItemData() const { return CurrentInventoryItemData ; }

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmountText;

private:
	FInventoryItemData CurrentInventoryItemData;
	class UInventoryWidget* ParentInventoryWidget = nullptr;
	class UEquipWidget* ParentEquipWidget = nullptr;
	FLinearColor DefaultBurshColor;
};
