// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "DialogueComponent.h"

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

    CollisionEnabledHasPhysics(ECollisionEnabled::NoCollision);

	if (NextButton) {
		NextButton->OnClicked.AddDynamic(this, &UDialogueWidget::OnNextButtonClicked);
	}
}

void UDialogueWidget::SetDialogueText(const FString& NewText)
{
	if (DialogueText) {
		DialogueText->SetText(FText::FromString(NewText));
	}
}

void UDialogueWidget::OnNextButtonClicked()
{
    if (auto* NPC = GetOwningPlayer()) {
        if (auto* DialogueComponent = NPC->FindComponentByClass<UDialogueComponent>()) {
            FString NextDialogue = DialogueComponent->GetNextDialogue();
            if (NextDialogue.Equals(TEXT("더 이상 대화가 없습니다."))) {
                if (NextButton) {
                    auto* ButtonText = Cast<UTextBlock>(NextButton->GetChildAt(0));
                    if (ButtonText) {
                        ButtonText->SetText(ComunicationEndMessage);
                    }
                }
            }
            else {
                SetDialogueText(NextDialogue);
            }
        }
    }
}
