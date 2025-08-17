// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "PlayerHUD.h"
#include "InventoryWidget.h"
#include "EquipWidget.h"
#include "SettingWidget.h"

void UMainMenuWidget::NativeConstruct()
{
    if (InventoryButton)
        InventoryButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenInventoryWidget);

    if (EquipButton)
        EquipButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenEquipWidget);

    if (SettingButton)
        SettingButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenSettingWidget);

    if (QuitGameButton) {
        //TODO: 게임 종료
    }
}

void UMainMenuWidget::OpenInventoryWidget()
{
    auto* PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!PlayerController)
        return;

    auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn());
    if (!PlayerCharacter)
        return;

    auto* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
    if (!PlayerHUD)
        return;

    auto* InventoryComponent = PlayerCharacter->GetInventoryComponent();
    if (!InventoryComponent)
        return;

	if (InventoryButton) {
        PlayerController->TryOpenInventory();
	}
}

void UMainMenuWidget::OpenEquipWidget()
{
    auto* PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!PlayerController)
        return;

    auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn());
    if (!PlayerCharacter)
        return;

    auto* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
    if (!PlayerHUD)
        return;

	if (EquipButton) {
        //TODO:위젯 만들어지는대로 넣기

	}
}

void UMainMenuWidget::OpenSettingWidget()
{
    auto* PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!PlayerController)
        return;

    auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn());
    if (!PlayerCharacter)
        return;

    auto* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
    if (!PlayerHUD)
        return;

	if (SettingButton) {
        //TODO:위젯 만들어지는대로 넣기

	}
}

void UMainMenuWidget::OpenQuitGameWidget()
{
	if (QuitGameButton) {
        //TODO:저장하고 종료
	}
}
