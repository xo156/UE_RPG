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

	//슬롯 생성
	void CreateAllSlots();
	
	//생성된 슬롯 등록하기
	void RegisterSlot(bool bIsEquipInventory, UFocusableSlotWidget* FocusableSlot, int32 Row, int32 Col);
	UFocusableSlotWidget* GetSlotAtOffset(int32 dRow, int32 dCol);

	//업데이트
	void UpdateAllPanels();
	void UpdateEquipPanel();
	void UpdateQuickPanel();

	//전용 인벤토리
	void UpdateInventoryPanel(class UInventoryComponent* InventoryComponent, EEquipSlotType TargetSlotType);
	void OpenEquipInventory();
	void CloseEquipInventory();

	//슬롯이 클릭될 때
	UFUNCTION()
	void HandleEquipSlotClicked(class UFocusableSlotWidget* ClickedSlot);
	UFUNCTION()
	void HandleEquipInventorySlotClicked(class UFocusableSlotWidget* ClickedSlot);

protected:
	//방향에 따른 포커싱 이동
	virtual void MoveFocusUp() override;
	virtual void MoveFocusDown() override;
	virtual void MoveFocusLeft() override;
	virtual void MoveFocusRight() override;

	//각 패널
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* SlotGridPanel; //슬롯 배치 패널

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* EquipInventorySlotGridPanel; //전용 인벤토리 슬롯 배치 패널

	//슬롯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSubclassOf<class UEquipSlotWidget> EquipSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSubclassOf<class UQuickSlotWidget> QuickSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSubclassOf<class UInventorySlotWidget> InventorySlotWidgetClass;

private:
	//장비창
	TArray<UFocusableSlotWidget*> EquipmentSlots; //전체 슬롯
	TMap<UFocusableSlotWidget*, FIntPoint> EquipSlotPositions;
	TMap<FIntPoint, UFocusableSlotWidget*> EquipPositionToSlot;

	//전용 인벤토리
	TArray<UInventorySlotWidget*> EquipInventorySlots; //전체 슬롯
	TMap<UFocusableSlotWidget*, FIntPoint> EquipInventorySlotPositions;
	TMap<FIntPoint, UFocusableSlotWidget*> EquipInventoryPositionToSlot;

	//포커싱
	UFocusableSlotWidget* CurrentFocusedSlot; //현재 포커싱중인 슬롯
	UEquipSlotWidget* CurrentClickedEquipSlot; //현재 클릭 된 슬롯

	//현재 활성화된 슬롯 맵
	TMap<UFocusableSlotWidget*, FIntPoint>* ActiveSlotPositions;
	TMap<FIntPoint, UFocusableSlotWidget*>* ActivePositionToSlot;

	//컴포넌트
	class UEquipComponent* EquipComponent;
	class UQuickSlotComponent* QuickSlotComponent;
};
