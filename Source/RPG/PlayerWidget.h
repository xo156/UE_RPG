// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable, Category = "Status")
    void UpdateHP(float CurrentHP, float MaxHP);

    UFUNCTION(BlueprintCallable, Category = "Status")
    void UpdateMP(float CurrentMP, float MaxMP);

    UFUNCTION(BlueprintCallable, Category = "Status")
    void UpdateStamina(float CurrentStamina, float MaxStamina);

    UFUNCTION()
    void OnPlayerStatusUswerWidgetUpdate(float NewHP, float NewMP, float NewStamina);

protected:
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HPProgressBar;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* MPProgressBar;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* StaminaProgressBar;
};
