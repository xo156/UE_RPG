// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemData.h"
#include "InventoryItemAction.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UInventoryItemAction : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();

	UPROPERTY(meta = (BindWidget))
	class UButton* OnlyUse;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* OnlyUseText;

	UPROPERTY(meta = (BindWidget))
	class UButton* OnlyDestroy;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* OnlyDestroyText;
	
	void SetItemData(const FInventoryItemData& InItemData);
	FInventoryItemData InventoryItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UDataTable* ItemDataTable;

	UFUNCTION()
	void OnOnlyUseClicked();

	UFUNCTION()
	void OnOnlyDestroyClicked();
};
