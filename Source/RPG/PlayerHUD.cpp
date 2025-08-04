// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "PlayerWidget.h"
#include "ItemData.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "InventoryItemData.h"
#include "InventoryQuickSlotWidget.h"
#include "MyCharacter.h"
#include "HPActorComponent.h"
#include "StaminaActorComponent.h"

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
            InventoryWidgetInstance->CreateInventorySlotWidget(MyCharacter->GetInventoryComponent());
            InventoryWidgetInstance->AddToViewport();
            InventoryWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    //퀵 슬롯 표시
    if (InventoryQuickSlotWidgetClass) {
        InventoryQuickSlotWidgetInstance = CreateWidget<UInventoryQuickSlotWidget>(GetWorld(), InventoryQuickSlotWidgetClass);
        if (InventoryQuickSlotWidgetInstance) {
            InventoryQuickSlotWidgetInstance->AddToViewport();
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

UInventorySlotWidget* APlayerHUD::GetInventorySlotWidget()
{
    return InventorySlotWidgetInstance ? InventorySlotWidgetInstance : nullptr;
}

UInventoryQuickSlotWidget* APlayerHUD::GetQuickSlotWidget()
{
    return InventoryQuickSlotWidgetInstance ? InventoryQuickSlotWidgetInstance : nullptr;
}
