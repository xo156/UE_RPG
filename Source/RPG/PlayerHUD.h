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
    void InitWidget();

    void OpenInventory(class UInventoryComponent* InventoryComponent);
    void CloseInventory(class UInventoryComponent* InventoryComponent);

    class UInventorySlotWidget* GetInventorySlotWidget();
    class UInventoryQuickSlotWidget* GetQuickSlotWidget();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UPlayerWidget> PlayerWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UInventoryQuickSlotWidget> InventoryQuickSlotWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UInventoryWidget> InventoryWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UInventoryTooltip> InventoryTooltipClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UInventoryItemAction> InventoryItemActionClass;

    //TODO: �� ���Ե� ����ϴµ� �κ��丮���� �����ϸ� ���⿡�� ����

private:
    UPlayerWidget* PlayerWidget;
    class UInventoryWidget* InventoryWidgetInstance;
    class UInventorySlotWidget* InventorySlotWidgetInstance;
    class UInventoryTooltip* InventoryTooltipInstance;
    class UInventoryItemAction* InventoryItemActionInstance;
    class UInventoryQuickSlotWidget* InventoryQuickSlotWidgetInstance;
    
};
