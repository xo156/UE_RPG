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
	void RegisterSlot(bool bIsEquipInventory, UFocusableSlotWidget* FocusableSlot, int32 Row, int32 Col);
	UFocusableSlotWidget* GetSlotAtOffset(int32 dRow, int32 dCol);

	//������Ʈ
	void UpdateAllPanels();
	void UpdateEquipPanel();
	void UpdateQuickPanel();

	//���� �κ��丮
	void UpdateInventoryPanel(class UInventoryComponent* InventoryComponent, EEquipSlotType TargetSlotType);
	void OpenEquipInventory();
	void CloseEquipInventory();

	//������ Ŭ���� ��
	UFUNCTION()
	void HandleEquipSlotClicked(class UFocusableSlotWidget* ClickedSlot);
	UFUNCTION()
	void HandleEquipInventorySlotClicked(class UFocusableSlotWidget* ClickedSlot);

protected:
	//���⿡ ���� ��Ŀ�� �̵�
	virtual void MoveFocusUp() override;
	virtual void MoveFocusDown() override;
	virtual void MoveFocusLeft() override;
	virtual void MoveFocusRight() override;

	//�� �г�
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* SlotGridPanel; //���� ��ġ �г�

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* EquipInventorySlotGridPanel; //���� �κ��丮 ���� ��ġ �г�

	//���� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSubclassOf<class UEquipSlotWidget> EquipSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSubclassOf<class UQuickSlotWidget> QuickSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSubclassOf<class UInventorySlotWidget> InventorySlotWidgetClass;

private:
	//���â
	TArray<UFocusableSlotWidget*> EquipmentSlots; //��ü ����
	TMap<UFocusableSlotWidget*, FIntPoint> EquipSlotPositions;
	TMap<FIntPoint, UFocusableSlotWidget*> EquipPositionToSlot;

	//���� �κ��丮
	TArray<UInventorySlotWidget*> EquipInventorySlots; //��ü ����
	TMap<UFocusableSlotWidget*, FIntPoint> EquipInventorySlotPositions;
	TMap<FIntPoint, UFocusableSlotWidget*> EquipInventoryPositionToSlot;

	//��Ŀ��
	UFocusableSlotWidget* CurrentFocusedSlot; //���� ��Ŀ������ ����
	UEquipSlotWidget* CurrentClickedEquipSlot; //���� Ŭ�� �� ����

	//���� Ȱ��ȭ�� ���� ��
	TMap<UFocusableSlotWidget*, FIntPoint>* ActiveSlotPositions;
	TMap<FIntPoint, UFocusableSlotWidget*>* ActivePositionToSlot;

	//������Ʈ
	class UEquipComponent* EquipComponent;
	class UQuickSlotComponent* QuickSlotComponent;
};
