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

protected:
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UPlayerWidget> PlayerWidgetClass;

private:
    UPROPERTY()
    UPlayerWidget* PlayerWidget;
};
