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

    void OpenInventory(class UInventoryComponent* InventoryComponent);
    void CloseInventory(class UInventoryComponent* InventoryComponent);

    void OpenEquipWidget();
    void CloseEquipWidget();

    void OpenSettingWidget();
    void CloseSettingWidget();

    void OpenMainMenuWidget();
    void CloseMainMenuWidget();

    class UInventorySlotWidget* GetInventorySlotWidget();
    class UInventoryQuickSlotWidget* GetQuickSlotWidget();

    class UInventoryWidget* GetInventoryWidget() const { return InventoryWidgetInstance; }

    class ULockonWidget* GetLockonWidget() const { return LockonWidgetInstance; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UPlayerWidget> PlayerWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UInventoryWidget> InventoryWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UEquipWidget> EquipWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class USettingWidget> SettingWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UInventoryQuickSlotWidget> InventoryQuickSlotWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class ULockonWidget> LockonWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UMainMenuWidget> MainMenuWidgetClass;

private:
    UPlayerWidget* PlayerWidget;
    class UInventoryWidget* InventoryWidgetInstance;
    class UInventorySlotWidget* InventorySlotWidgetInstance;
    class UEquipWidget* EquipWidgetInstance;
    class USettingWidget* SettingWidgetInstance;
    class UInventoryQuickSlotWidget* InventoryQuickSlotWidgetInstance;
    class ULockonWidget* LockonWidgetInstance;
    class UMainMenuWidget* MainMenuWidgetInstance;
};
