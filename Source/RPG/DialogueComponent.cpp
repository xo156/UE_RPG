// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueComponent.h"
#include "DialogueWidget.h"
#include "DialogueTable.h"

// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

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
		DialogueWidgetInstance = CreateWidget<UDialogueWidget>(GetWorld(), DialogueWidgetClass);
		if (DialogueWidgetInstance) {
			DialogueWidgetInstance->AddToViewport();
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
	return FString("더 이상 대화가 없습니다.");
}

