// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FocusableSlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, UFocusableSlotWidget*, ClickedSlot);

UCLASS()
class RPG_API UFocusableSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void SetIsFocused(bool bNewFocus);
	bool IsFocused() const { return bIsFocused; }

	void SetOwnerWidget(class UEquipWidget* InOwner);

	virtual void ClearSlot();

	UFUNCTION()
	void HandleClicked();
	FOnSlotClicked OnSlotClicked; //슬롯 클릭 이벤트

protected:
	UPROPERTY(meta = (BindWidget))
	class UBorder* FocusBorder;

	UPROPERTY(meta = (BindWidget))
	class UButton* SlotButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
	FLinearColor FocusedColor = FLinearColor::Red;
	FLinearColor UnFocusedColor;

	bool bIsFocused = false;
	class UEquipWidget* OwnerWidget;

};
