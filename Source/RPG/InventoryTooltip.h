// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryTooltip.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UInventoryTooltip : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void PreConstruct(bool bIsDesignTime);

	void InitTooltip(const struct FItemData& InItemData);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDescription;
};
