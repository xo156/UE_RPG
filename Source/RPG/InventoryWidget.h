// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FocusableUserWidget.h"
#include "ItemData.h"
#include "InventoryItemData.h"
#include "InventoryWidget.generated.h"


UCLASS()
class RPG_API UInventoryWidget : public UFocusableUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void InitInventoryWidget(class UInventoryComponent* InventoryComponent);

	//슬롯
	void CreateInventorySlotWidget(class UInventoryComponent* InventoryComponent);
	void UpdateInventorySlotWidget(class UInventoryComponent* InventoryComponent);

	void ConfirmFocusSlot();
	class UInventorySlotWidget* GetCurrentFocusedSlot() const { return Cast<UInventorySlotWidget>(CurrentFocusedSlot); }

	//툴팁
	void InitTooltip(const FItemData& InItemData);
	void ClearToolTip();

	//액션
	void InitItemAction(const FInventoryItemData& InItemData);
	void ClearItemAction();
	UFUNCTION()
	void OnUseClicked();

	UFUNCTION()
	void OnDestroyClicked();

protected:
	//방향에 따른 포커싱 이동
	virtual void MoveFocusUp() override;
	virtual void MoveFocusDown() override;
	virtual void MoveFocusLeft() override;
	virtual void MoveFocusRight() override;

	//인벤토리 메인
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InventoryName;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* InventoryGridPanel;

	//인벤토리 슬롯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<class UInventorySlotWidget> InventorySlotWidgetClass;

	//툴팁
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AttackPowerValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DefensePowerValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDescription;

	//아이템 액션
	UPROPERTY(meta = (BindWidget))
	class UButton* UseButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UseText;

	UPROPERTY(meta = (BindWidget))
	class UButton* DestroyButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DestroyText;

	//인벤토리 아이템 데이터
	FInventoryItemData InventoryItemData;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess="true"))
	int32 MaxDesiredColumns = 6;
	int32 InventoryColumnCount = 0;
	TArray<class UInventorySlotWidget*> InventorySlotWidgets;
};
