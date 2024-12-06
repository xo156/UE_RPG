// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void CreateInventoryWidget(class UInventoryComponent* InventoryComponent);
	void UpdateInventoryWidget(class UInventoryComponent* InventoryComponent);

	void SetPlayerMoney(int32 PlayerMoney);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InventoryName;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* InventorySlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<class UInventorySlotWidget> InventorySlotWidgetClass;
	class UInventorySlotWidget* InventorySlotWidgetInstance;
};
