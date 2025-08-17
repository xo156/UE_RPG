// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FocusableSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UFocusableSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void SetIsFocused(bool bNewFocus);
	bool IsFocused() const { return bIsFocused; }

	virtual void OnSlotConfirmed();

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* SlotButton;

	bool bIsFocused = false;
};
