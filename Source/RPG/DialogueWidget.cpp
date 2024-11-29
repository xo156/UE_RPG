// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "DialogueComponent.h"

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UDialogueWidget::SetDialogueText(const FString& NewText)
{
	if (DialogueText) {
		DialogueText->SetText(FText::FromString(NewText));
	}
}
