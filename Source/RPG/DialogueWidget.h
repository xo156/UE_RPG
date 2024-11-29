// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(Category = "Dialogue")
    void SetDialogueText(const FString& NewText);

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DialogueText;

    UPROPERTY(meta = (BindWidget))
    class UButton* NextButton;

    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void OnNextButtonClicked();

    UPROPERTY()
    FText ComunicationEndMessage;
	
};
