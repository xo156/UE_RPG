// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FocusableUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UFocusableUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void RequestFocus(class UFocusableSlotWidget* NewFocusSlot);
    virtual void MoveFocus(const FVector2D& Direction);
    void ConfirmFocusSlot();

protected:
    //방향에 따른 포커싱 이동
    virtual void MoveFocusUp();
    virtual void MoveFocusDown();
    virtual void MoveFocusLeft();
    virtual void MoveFocusRight();

    class UFocusableSlotWidget* CurrentFocusedSlot = nullptr;

    TArray<TArray<class UFocusableSlotWidget*>> FocusablePanels;
    TArray<int32> PanelColumnCounts;
};
