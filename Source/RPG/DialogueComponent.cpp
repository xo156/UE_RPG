// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueComponent.h"
#include "DialogueWidget.h"
#include "DialogueTable.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	CurrentIndex = 0;
}


// Called when the game starts
void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

}

void UDialogueComponent::CreateDialogueWidget(UDataTable* InDialogueTable)
{
	DialogueTable = InDialogueTable;
	LoadDialogues(DialogueTable);

	if (DialogueWidgetClass) {
		DialogueWidgetInstance = CreateWidget<UDialogueWidget>(OwnerCharacter->GetWorld(), DialogueWidgetClass);
		if (DialogueWidgetInstance) {
			DialogueWidgetInstance->SetDialogueText(GetNextDialogue());
			DialogueWidgetInstance->AddToViewport();
			DialogueWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UDialogueComponent::LoadDialogues(UDataTable* InDialogueTable)
{
	if (InDialogueTable) {
		InDialogueTable->GetAllRows<FDialogueTable>(TEXT("Dialogues Context String"), Dialogues);
	}
}

FString UDialogueComponent::GetNextDialogue()
{
	if (CurrentIndex < Dialogues.Num()) {
		return Dialogues[CurrentIndex++]->Content;
	}
	else {
		CheckEndDialogueMessage();
		return FString("");
	}
}

void UDialogueComponent::CheckEndDialogueMessage()
{
	if (DialogueWidgetInstance) {
		if (auto* PlayerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0))) {
			PlayerCharacter->bIsTalk = false;
		}
		DialogueWidgetInstance->RemoveFromViewport();
		DialogueWidgetInstance = nullptr;
		CurrentIndex = 0;
	}
}

void UDialogueComponent::SetOwnerCharacter(AActor* NewOwnerCharacter)
{
	if (NewOwnerCharacter)
		OwnerCharacter = NewOwnerCharacter;
}

UDialogueWidget* UDialogueComponent::GetDialogueWidgetInstance()
{
	return DialogueWidgetInstance ? DialogueWidgetInstance : nullptr;
}

