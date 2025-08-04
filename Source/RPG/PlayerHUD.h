// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    //void InitWidget();

    void OpenInventory(class UInventoryComponent* InventoryComponent);
    void CloseInventory(class UInventoryComponent* InventoryComponent);

    class UInventorySlotWidget* GetInventorySlotWidget();
    class UInventoryQuickSlotWidget* GetQuickSlotWidget();

    class UInventoryWidget* GetInventoryWidget() const { return InventoryWidgetInstance; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UPlayerWidget> PlayerWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UInventoryQuickSlotWidget> InventoryQuickSlotWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UInventoryWidget> InventoryWidgetClass;

    //TODO: 퀵 슬롯도 등록하는데 인벤토리에서 딸깍하면 여기에서 하자

private:
    UPlayerWidget* PlayerWidget;
    class UInventoryWidget* InventoryWidgetInstance;
    class UInventorySlotWidget* InventorySlotWidgetInstance;
    class UInventoryQuickSlotWidget* InventoryQuickSlotWidgetInstance;
    
};
