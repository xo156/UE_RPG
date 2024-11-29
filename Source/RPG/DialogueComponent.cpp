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

void UDialogueComponent::LoadDialogues(TArray<FDialogueTable*> InDialogues)
{
	Dialogues = InDialogues;
}

FString UDialogueComponent::GetNextDialogue()
{
	if (CurrentIndex < Dialogues.Num()) {
		return Dialogues[CurrentIndex++]->Content;
	}
	return FString("더 이상 대화가 없습니다.");
}

