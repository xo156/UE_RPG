// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "GameFramework/PlayerController.h"
#include "PlayerWidget.h"
#include "MyCharacter.h"
#include "HPActorComponent.h"
#include "StaminaActorComponent.h"

void APlayerHUD::BeginPlay()
{
    Super::BeginPlay();

    auto* PlayerController = GetOwningPlayerController();
    if (PlayerController && PlayerWidgetClass) {
        PlayerWidget = CreateWidget<UPlayerWidget>(PlayerController, PlayerWidgetClass);
        if (PlayerWidget) {
            PlayerWidget->AddToViewport();

            auto* MyCharacter = Cast<AMyCharacter>(PlayerController->GetPawn());
            if (MyCharacter) {
                auto* HPComp = MyCharacter->FindComponentByClass<UHPActorComponent>();
                auto* StaminaComp = MyCharacter->FindComponentByClass<UStaminaActorComponent>();
                PlayerWidget->BindToComponents(HPComp, StaminaComp);
            }
        }
    }
}
