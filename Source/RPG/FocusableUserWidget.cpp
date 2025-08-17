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
    //���1. ���⿡�� �� �ϱ�
    /*if (FocusablePanels.Num() == 0)
    //    return;

    ////���� ��Ŀ�� �г�/���� ã��
    //int32 CurrentPanelIndex = INDEX_NONE;
    //int32 CurrentSlotIndex = INDEX_NONE;

    //for (int32 PanelIndex = 0; PanelIndex < FocusablePanels.Num(); PanelIndex++) {
    //    CurrentSlotIndex = FocusablePanels[PanelIndex].IndexOfByKey(CurrentFocusedSlot);
    //    if (CurrentSlotIndex != INDEX_NONE) {
    //        CurrentPanelIndex = PanelIndex;
    //        break;
    //    }
    //}

    ////��Ŀ�� ������ ù ��������
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
    //    //���� �г� �� �̵�
    //    RequestFocus(FocusablePanels[CurrentPanelIndex][NewIndex]);
    //}
    //else {
    //    //�ٸ� �гη� �Ѿ���
    //    int32 NextPanelIndex = CurrentPanelIndex;

    //    if (Direction.Y > 0) {
    //        //���� �̵�
    //        NextPanelIndex = FMath::Max(CurrentPanelIndex - 1, 0);
    //    }
    //    else if (Direction.Y < 0) {
    //        //�Ʒ��� �̵�
    //        NextPanelIndex = FMath::Min(CurrentPanelIndex + 1, FocusablePanels.Num() - 1);
    //    }

    //    if (NextPanelIndex != CurrentPanelIndex && FocusablePanels[NextPanelIndex].Num() > 0) {
    //        //X ��ǥ�� �����ϸ� �̵�
    //        int32 TargetColCount = PanelColumnCounts[NextPanelIndex];
    //        int32 TargetX = FMath::Clamp(X, 0, TargetColCount - 1);
    //        int32 TargetIndex = TargetX; //ù �ٿ��� ��Ŀ��

    //        if (FocusablePanels[NextPanelIndex].IsValidIndex(TargetIndex))
    //            RequestFocus(FocusablePanels[NextPanelIndex][TargetIndex]);
    //        else
    //            RequestFocus(FocusablePanels[NextPanelIndex][0]);
    //    }
    }*/

    //���2. ���� ���� �ٸ� �Լ� ȣ���ϵ���
    //�׷��� ��� ������ �����ϰ� �������ٵ� �׳� �ϰ������ϸ� ���� �ʳ�?
    //virtual�Լ��� ���� �ʿ䵵 ����?
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
