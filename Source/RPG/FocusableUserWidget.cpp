// Fill out your copyright notice in the Description page of Project Settings.


#include "FocusableUserWidget.h"
#include "FocusableSlotWidget.h"

void UFocusableUserWidget::RequestFocus(UFocusableSlotWidget* NewFocusSlot)
{
    if (CurrentFocusedSlot == NewFocusSlot)
        return;

    if (CurrentFocusedSlot && CurrentFocusedSlot->IsFocused())
        CurrentFocusedSlot->SetIsFocused(false);

    CurrentFocusedSlot = NewFocusSlot;

    if (CurrentFocusedSlot)
        CurrentFocusedSlot->SetIsFocused(true);
}

void UFocusableUserWidget::MoveFocus(const FVector2D& Direction)
{
    //방법1. 여기에서 다 하기
    /*if (FocusablePanels.Num() == 0)
    //    return;

    ////현재 포커스 패널/슬롯 찾기
    //int32 CurrentPanelIndex = INDEX_NONE;
    //int32 CurrentSlotIndex = INDEX_NONE;

    //for (int32 PanelIndex = 0; PanelIndex < FocusablePanels.Num(); PanelIndex++) {
    //    CurrentSlotIndex = FocusablePanels[PanelIndex].IndexOfByKey(CurrentFocusedSlot);
    //    if (CurrentSlotIndex != INDEX_NONE) {
    //        CurrentPanelIndex = PanelIndex;
    //        break;
    //    }
    //}

    ////포커스 없으면 첫 슬롯으로
    //if (CurrentPanelIndex == INDEX_NONE) {
    //    RequestFocus(FocusablePanels[0][0]);
    //    return;
    //}

    //const int32 ColumnCount = PanelColumnCounts[CurrentPanelIndex];
    //const int32 TotalSlots = FocusablePanels[CurrentPanelIndex].Num();

    //int32 X = CurrentSlotIndex % ColumnCount;
    //int32 Y = CurrentSlotIndex / ColumnCount;

    //int32 NewX = FMath::Clamp(X + FMath::RoundToInt(Direction.X), 0, ColumnCount - 1);
    //int32 NewY = Y - FMath::RoundToInt(Direction.Y);

    //int32 NewIndex = NewY * ColumnCount + NewX;

    //if (FocusablePanels[CurrentPanelIndex].IsValidIndex(NewIndex)) {
    //    //같은 패널 내 이동
    //    RequestFocus(FocusablePanels[CurrentPanelIndex][NewIndex]);
    //}
    //else {
    //    //다른 패널로 넘어기기
    //    int32 NextPanelIndex = CurrentPanelIndex;

    //    if (Direction.Y > 0) {
    //        //위로 이동
    //        NextPanelIndex = FMath::Max(CurrentPanelIndex - 1, 0);
    //    }
    //    else if (Direction.Y < 0) {
    //        //아래로 이동
    //        NextPanelIndex = FMath::Min(CurrentPanelIndex + 1, FocusablePanels.Num() - 1);
    //    }

    //    if (NextPanelIndex != CurrentPanelIndex && FocusablePanels[NextPanelIndex].Num() > 0) {
    //        //X 좌표를 유지하며 이동
    //        int32 TargetColCount = PanelColumnCounts[NextPanelIndex];
    //        int32 TargetX = FMath::Clamp(X, 0, TargetColCount - 1);
    //        int32 TargetIndex = TargetX; //첫 줄에만 포커스

    //        if (FocusablePanels[NextPanelIndex].IsValidIndex(TargetIndex))
    //            RequestFocus(FocusablePanels[NextPanelIndex][TargetIndex]);
    //        else
    //            RequestFocus(FocusablePanels[NextPanelIndex][0]);
    //    }
    }*/

    //방법2. 방향 따라서 다른 함수 호출하도록
    //그런데 모든 위젯이 동일하게 움직일텐데 그냥 일괄적용하면 되지 않나?
    //virtual함수로 만들 필요도 없이?
    if (Direction.X > 0) {
        MoveFocusRight();
    }
    else if (Direction.X < 0) {
        MoveFocusLeft();
    }
    else if (Direction.Y > 0) {
        MoveFocusUp();
    }
    else if (Direction.Y < 0) {
        MoveFocusDown();
    }
}

void UFocusableUserWidget::ConfirmFocusSlot()
{
    if (!CurrentFocusedSlot)
        return;

    CurrentFocusedSlot->OnSlotConfirmed();
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
