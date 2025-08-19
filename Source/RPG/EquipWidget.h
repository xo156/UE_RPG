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
	void RegisterSlot(UFocusableSlotWidget* FocusableSlot, int32 Row, int32 Col);
	void UpdateFocus(UFocusableSlotWidget* NewFocusedSlot);
	UFocusableSlotWidget* GetSlotAtOffset(int32 dRow, int32 dCol);

	//슬롯이 클릭될 때
	UFUNCTION()
	void HandleEquipSlotClicked(UFocusableSlotWidget* ClickedSlot);
	UFUNCTION()
	void HandleInventorySlotClicked(class UInventorySlotWidget* InventorySlot);

protected:
	//방향에 따른 포커싱 이동
	virtual void MoveFocusUp() override;
	virtual void MoveFocusDown() override;
	virtual void MoveFocusLeft() override;
	virtual void MoveFocusRight() override;

	//각 패널
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* SlotGridPanel; //슬롯 배치 패널

	//슬롯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSubclassOf<class UEquipSlotWidget> EquipSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSubclassOf<class UQuickSlotWidget> QuickSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSubclassOf<class UInventorySlotWidget> InventorySlotWidgetClass;

private:
	TArray<UFocusableSlotWidget*> AllSlots; //전체 슬롯
	UFocusableSlotWidget* CurrentFocusedSlot; //현재 포커싱중인 슬롯
	UEquipSlotWidget* CurrentClickedEquipSlot; //현재 클릭 된 슬롯

	//슬롯 좌표 매핑
	TMap<UFocusableSlotWidget*, FIntPoint> SlotPositions;
	TMap<FIntPoint, UFocusableSlotWidget*> PositionToSlot;

	TArray<UInventorySlotWidget*> EquipInventorySlots; //전용 인벤토리 슬롯들
	class UInventoryComponent* InventoryComponent;
};
