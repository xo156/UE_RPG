// Fill out your copyright notice in the Description page of Project Settings.


#include "FocusableSlotWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"

void UFocusableSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotButton)
		SlotButton->OnClicked.AddDynamic(this, &UFocusableSlotWidget::HandleClicked);

	UnFocusedColor = FocusBorder->GetBrushColor();
}

void UFocusableSlotWidget::SetIsFocused(bool bNewFocus)
{
	if (!FocusBorder)
		return;

	bIsFocused = bNewFocus;

	if (bIsFocused) {
		FocusBorder->SetBrushColor(FLinearColor::Red);
	}
	else {
		FocusBorder->SetBrushColor(UnFocusedColor);
	}
}

void UFocusableSlotWidget::SetOwnerWidget(UEquipWidget* InOwner)
{
	if(InOwner)
		OwnerWidget = InOwner;
}

void UFocusableSlotWidget::SetItemIcon(UTexture2D* ItemIcon)
{
	if (!ItemIcon)
		return;

	FButtonStyle ButtonStyle = SlotButton->GetStyle();
	FSlateBrush NewBrush;
	NewBrush.SetResourceObject(ItemIcon);

	ButtonStyle.SetNormal(NewBrush);
	ButtonStyle.SetHovered(NewBrush);
	ButtonStyle.SetPressed(NewBrush);
	SlotButton->SetStyle(ButtonStyle);
}

void UFocusableSlotWidget::ClearSlot()
{
	FButtonStyle ButtonStyle = SlotButton->GetStyle();
	FSlateBrush NewBrush;
	NewBrush.SetResourceObject(nullptr);

	ButtonStyle.SetNormal(NewBrush);
	ButtonStyle.SetHovered(NewBrush);
	ButtonStyle.SetPressed(NewBrush);
	SlotButton->SetStyle(ButtonStyle);
}

void UFocusableSlotWidget::HandleClicked()
{
	OnSlotClicked.Broadcast(this);
}