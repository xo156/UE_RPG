// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "PlayerWidget.h"
#include "ItemData.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "EquipWidget.h"
#include "SettingWidget.h"
#include "InventoryItemData.h"
#include "InventoryQuickSlotWidget.h"
#include "MyCharacter.h"
#include "HPActorComponent.h"
#include "StaminaActorComponent.h"
#include "LockonWidget.h"
#include "MainMenuWidget.h"
#include "FocusableUserWidget.h"
#include "EquipComponent.h"

void APlayerHUD::BeginPlay()
{
    Super::BeginPlay();

    auto* PlayerController = GetOwningPlayerController();
    if (!PlayerController)
        return;

    auto* MyCharacter = Cast<AMyCharacter>(PlayerController->GetPawn());
    if (!MyCharacter)
        return;

    //체력, 스테미나 표시
    if (PlayerWidgetClass) {
        PlayerWidget = CreateWidget<UPlayerWidget>(PlayerController, PlayerWidgetClass);
        if (PlayerWidget) {
            PlayerWidget->AddToViewport();

        auto* HPComponent = MyCharacter->FindComponentByClass<UHPActorComponent>();
        auto* StaminaComponent = MyCharacter->FindComponentByClass<UStaminaActorComponent>();
        PlayerWidget->BindToComponents(HPComponent, StaminaComponent);
        }
    }

    //인벤토리 표시
    if (InventoryWidgetClass) {
        InventoryWidgetInstance = CreateWidget<UInventoryWidget>(PlayerController, InventoryWidgetClass);
        if (InventoryWidgetInstance) {
            InventoryWidgetInstance->InitInventoryWidget(MyCharacter->GetInventoryComponent());
            InventoryWidgetInstance->AddToViewport();
            InventoryWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    //장비창
    if (EquipWidgetClass) {
        EquipWidgetInstance = CreateWidget<UEquipWidget>(GetWorld(), EquipWidgetClass);
        if (EquipWidgetInstance) {
            EquipWidgetInstance->AddToViewport();
            EquipWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    //세팅창
    if (SettingWidgetClass) {
        SettingWidgetInstance = CreateWidget<USettingWidget>(GetWorld(), SettingWidgetClass);
        if (SettingWidgetInstance) {
            SettingWidgetInstance->AddToViewport();
            SettingWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    //퀵 슬롯 표시
    if (InventoryQuickSlotWidgetClass) {
        InventoryQuickSlotWidgetInstance = CreateWidget<UInventoryQuickSlotWidget>(GetWorld(), InventoryQuickSlotWidgetClass);
        if (InventoryQuickSlotWidgetInstance) {
            InventoryQuickSlotWidgetInstance->AddToViewport();
        }
    }

    //락 온
    if (LockonWidgetClass) {
        LockonWidgetInstance = CreateWidget<ULockonWidget>(GetWorld(), LockonWidgetClass);
        if (LockonWidgetInstance) {
            LockonWidgetInstance->AddToViewport();
            LockonWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    //메인 메뉴
    if (MainMenuWidgetClass) {
        LockonWidgetInstance = CreateWidget<ULockonWidget>(GetWorld(), MainMenuWidgetClass);
        if (MainMenuWidgetInstance) {
            MainMenuWidgetInstance->AddToViewport();
            MainMenuWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        }
    }

}

void APlayerHUD::OpenInventory(UInventoryComponent* InventoryComponent)
{
    if (!InventoryWidgetInstance || !InventoryComponent)
        return;

    const bool bIsCurrentlyOpen = InventoryComponent->IsOpen();
    auto* PlayerController = GetOwningPlayerController();
    if (!PlayerController)
        return;

    if (!bIsCurrentlyOpen) {
        InventoryWidgetInstance->UpdateInventorySlotWidget(InventoryComponent);
        InventoryWidgetInstance->SetVisibility(ESlateVisibility::Visible);
        PlayerController->bShowMouseCursor = true;
        InventoryComponent->SetIsOpen(true);
    }
}

void APlayerHUD::CloseInventory(UInventoryComponent* InventoryComponent)
{
    if (!InventoryWidgetInstance || !InventoryComponent)
        return;

    const bool bIsCurrentlyOpen = InventoryComponent->IsOpen();
    auto* PlayerController = GetOwningPlayerController();
    if (!PlayerController)
        return;

    if (bIsCurrentlyOpen) {
        InventoryWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        PlayerController->bShowMouseCursor = false;
        InventoryComponent->SetIsOpen(false);
    }
}

void APlayerHUD::OpenEquipWidget(class UEquipComponent* EquipComponent)
{
    if (!EquipWidgetInstance)
        return;

    const bool bIsCurrentlyOpen = EquipComponent->IsOpen();
    auto* PlayerController = GetOwningPlayerController();
    if (!PlayerController)
        return;

    if (!bIsCurrentlyOpen) {
        //TODO: 전체 패널들 업데이트 하는 함수 제작
        EquipWidgetInstance->SetVisibility(ESlateVisibility::Visible);
        PlayerController->bShowMouseCursor = true;
        EquipComponent->SetIsOpen(true);
    }
}

void APlayerHUD::CloseEquipWidget(class UEquipComponent* EquipComponent)
{
    if (!EquipWidgetInstance)
        return;

    const bool bIsCurrentlyOpen = EquipComponent->IsOpen();
    auto* PlayerController = GetOwningPlayerController();
    if (!PlayerController)
        return;

    if (bIsCurrentlyOpen) {
        EquipWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        PlayerController->bShowMouseCursor = false;
        EquipComponent->SetIsOpen(false);
    }
}

void APlayerHUD::OpenSettingWidget()
{
    if (!SettingWidgetInstance)
        return;


}

void APlayerHUD::CloseSettingWidget()
{
    if (!SettingWidgetInstance)
        return;


}

void APlayerHUD::OpenMainMenuWidget()
{
    if (!MainMenuWidgetInstance /*|| !MainMenuComponent*/ )
        return;

    //const bool bIsCurrentlyOpen = InventoryComponent->IsOpen();
    auto* PlayerController = GetOwningPlayerController();
    if (!PlayerController)
        return;

    /*if (!bIsCurrentlyOpen) {
        InventoryWidgetInstance->UpdateInventorySlotWidget(InventoryComponent);
        InventoryWidgetInstance->SetVisibility(ESlateVisibility::Visible);
        PlayerController->bShowMouseCursor = true;
        InventoryComponent->SetIsOpen(true);
    }*/
}

void APlayerHUD::CloseMainMenuWidget()
{
}

UInventorySlotWidget* APlayerHUD::GetInventorySlotWidget()
{
    return InventorySlotWidgetInstance ? InventorySlotWidgetInstance : nullptr;
}

UInventoryQuickSlotWidget* APlayerHUD::GetQuickSlotWidget()
{
    return InventoryQuickSlotWidgetInstance ? InventoryQuickSlotWidgetInstance : nullptr;
}

UFocusableUserWidget* APlayerHUD::GetCurrentFocusableWidget() const
{
    if (InventoryWidgetInstance && InventoryWidgetInstance->IsInViewport())
        return InventoryWidgetInstance;

    if (EquipWidgetInstance && EquipWidgetInstance->IsInViewport())
        return EquipWidgetInstance;

    return nullptr;
}
