// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "InventoryItemData.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void InitInventoryWidget(class UInventoryComponent* InventoryComponent);

	//����
	void CreateInventorySlotWidget(class UInventoryComponent* InventoryComponent);
	void UpdateInventorySlotWidget(class UInventoryComponent* InventoryComponent);
	void RequestFocus(class UInventorySlotWidget* NewfocusSlot);
	void MoveFocus(const FVector2D& Direction);
	void ConfirmFocusSlot();
	class UInventorySlotWidget* GetCurrentFocusedSlot() const { return CurrentFocusedSlot; }

	//����
	void InitTooltip(const FItemData& InItemData);
	void ClearToolTip();

	//�׼�
	void InitItemAction(const FInventoryItemData& InItemData);
	void ClearItemAction();
	UFUNCTION()
	void OnUseClicked();

	UFUNCTION()
	void OnDestroyClicked();

	UFUNCTION()
	void OnQuickSlotClicked();

protected:
	//�κ��丮 ����
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InventoryName;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* InventorySlots;

	//�κ��丮 ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<class UInventorySlotWidget> InventorySlotWidgetClass;

	//����
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AttackPowerValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DefensePowerValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDescription;

	//������ �׼�
	UPROPERTY(meta = (BindWidget))
	class UButton* UseButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UseText;

	UPROPERTY(meta = (BindWidget))
	class UButton* DestroyButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DestroyText;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuickSlot;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuickSlotText;

	FInventoryItemData InventoryItemData;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess="true"))
	int32 MaxDesiredColumns = 6;
	int32 InventoryColumnCount;
	TArray<class UInventorySlotWidget*> InventorySlotWidgets;
	class UInventorySlotWidget* CurrentFocusedSlot = nullptr;
};
