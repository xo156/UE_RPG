// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FocusableUserWidget.h"
#include "EquipWidget.generated.h"

USTRUCT()
struct FEquipPanelInfo
{
	GENERATED_BODY()

	class UUniformGridPanel* Panel = nullptr;

	int32 ColumnCount = 1;
	int32 RowCount = 1;

	TArray<class UFocusableSlotWidget*> Slots;
};

UCLASS()
class RPG_API UEquipWidget : public UFocusableUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	//���â
	void CreateWeaponSlots();
	void CreateArmorSlots();
	void CreateQuickSlots();

	//�κ��丮
	void CreateEquipInventorySlots(int32 NumColumns, int32 NumRows);
	void UpdateEquipInventory(EEquipSlotType SelectedSlotType);
	void OnEquipInventorySlotClicked(class UInventorySlotWidget* ClickedSlot);

protected:
	//���⿡ ���� ��Ŀ�� �̵�
	virtual void MoveFocusUp() override;
	virtual void MoveFocusDown() override;
	virtual void MoveFocusLeft() override;
	virtual void MoveFocusRight() override;

	//�� �г�
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* WeaponGridPanel;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* ArmorGridPanel;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* QuickSlotGridPanel;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* EquipInventoryGridPanel;

	//���� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSubclassOf<class UEquipSlotWidget> EquipSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSubclassOf<class UQuickSlotWidget> QuickSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSubclassOf<class UInventorySlotWidget> InventorySlotWidgetClass;

private:
	TArray<FEquipPanelInfo> Panels; //�гε�

	TArray<UInventorySlotWidget*> EquipInventorySlots; //���� �κ��丮 ���Ե�
	class UInventoryComponent* InventoryComponent;

	//���� ��Ŀ��
	int32 CurrentPanelIndex = 0;
	int32 CurrentSlotIndex = 0;
};
