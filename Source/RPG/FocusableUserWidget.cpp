// Fill out your copyright notice in the Description page of Project Settings.


#include "FocusableUserWidget.h"
#include "FocusableSlotWidget.h"

void UFocusableUserWidget::RequestFocus(UFocusableSlotWidget* NewFocusSlot)
{
    if (CurrentFocusedSlot == NewFocusSlot)
        return;

    //기존의 포커스는 해제
    if (CurrentFocusedSlot && CurrentFocusedSlot->IsFocused())
        CurrentFocusedSlot->SetIsFocused(false);

    CurrentFocusedSlot = NewFocusSlot;

    //새롭게 포커스 설정
    if (CurrentFocusedSlot)
        CurrentFocusedSlot->SetIsFocused(true);
}

void UFocusableUserWidget::MoveFocus(const FVector2D& Direction)
{
    UE_LOG(LogTemp, Log, TEXT("void UFocusableUserWidget::MoveFocus()"));
    if (Direction.X > 0.1f) {
        UE_LOG(LogTemp, Log, TEXT("void UFocusableUserWidget::MoveFocusRight"));
        MoveFocusRight();
    }
    else if (Direction.X < -0.1f) {
        UE_LOG(LogTemp, Log, TEXT("void UFocusableUserWidget::MoveFocusLeft"));
        MoveFocusLeft();
    }
    else if (Direction.Y > 0.1f) {
        UE_LOG(LogTemp, Log, TEXT("void UFocusableUserWidget::MoveFocusUp"));
        MoveFocusUp();
    }
    else if (Direction.Y < -0.1f) {
        UE_LOG(LogTemp, Log, TEXT("void UFocusableUserWidget::MoveFocusDown"));
        MoveFocusDown();
    }
}

void UFocusableUserWidget::ConfirmFocusSlot()
{
    if (!CurrentFocusedSlot)
        return;

    CurrentFocusedSlot->HandleClicked();
    UE_LOG(LogTemp, Log, TEXT("Confirmed slot: %s"), *CurrentFocusedSlot->GetName());
}

void UFocusableUserWidget::MoveFocusUp()
{
}

void UFocusableUserWidget::MoveFocusDown()
{
}

void UFocusableUserWidget::MoveFocusLeft()
{
}

void UFocusableUserWidget::MoveFocusRight()
{
}
