// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipSlotType.h"
#include "FocusableUserWidget.h"
#include "EquipWidget.generated.h"

UCLASS()
class RPG_API UEquipWidget : public UFocusableUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	//���� ����
	void CreateAllSlots();
	
	//������ ���� ����ϱ�
	void RegisterSlot(UFocusableSlotWidget* FocusableSlot, int32 Row, int32 Col);
	void UpdateFocus(UFocusableSlotWidget* NewFocusedSlot);
	UFocusableSlotWidget* GetSlotAtOffset(int32 dRow, int32 dCol);

	//������ Ŭ���� ��
	UFUNCTION()
	void HandleEquipSlotClicked(UFocusableSlotWidget* ClickedSlot);
	UFUNCTION()
	void HandleInventorySlotClicked(class UInventorySlotWidget* InventorySlot);

protected:
	//���⿡ ���� ��Ŀ�� �̵�
	virtual void MoveFocusUp() override;
	virtual void MoveFocusDown() override;
	virtual void MoveFocusLeft() override;
	virtual void MoveFocusRight() override;

	//�� �г�
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* SlotGridPanel; //���� ��ġ �г�

	//���� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSubclassOf<class UEquipSlotWidget> EquipSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSubclassOf<class UQuickSlotWidget> QuickSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSubclassOf<class UInventorySlotWidget> InventorySlotWidgetClass;

private:
	TArray<UFocusableSlotWidget*> AllSlots; //��ü ����
	UFocusableSlotWidget* CurrentFocusedSlot; //���� ��Ŀ������ ����
	UEquipSlotWidget* CurrentClickedEquipSlot; //���� Ŭ�� �� ����

	//���� ��ǥ ����
	TMap<UFocusableSlotWidget*, FIntPoint> SlotPositions;
	TMap<FIntPoint, UFocusableSlotWidget*> PositionToSlot;

	TArray<UInventorySlotWidget*> EquipInventorySlots; //���� �κ��丮 ���Ե�
	class UInventoryComponent* InventoryComponent;
};
