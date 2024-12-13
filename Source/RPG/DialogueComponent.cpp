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

void UDialogueComponent::CreateDialogueWidget()
{
	if (DialogueWidgetClass) {
		DialogueWidgetInstance = CreateWidget<UDialogueWidget>(GetWorld(), DialogueWidgetClass);
		if (DialogueWidgetInstance) {
			if (DialogueCache.Contains(CurrentIndex)) { // 캐시가 유효한지 확인
				DialogueWidgetInstance->SetDialogueText(DialogueCache[CurrentIndex]->TableContent);
				DialogueWidgetInstance->AddToViewport();
				DialogueWidgetInstance->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UDialogueComponent::NextDialogue()
{
	if (DialogueCache.Contains(++CurrentIndex)) {
		DialogueWidgetInstance->SetDialogueText(DialogueCache[CurrentIndex]->TableContent);
	}
	else {
		DialogueWidgetInstance->RemoveFromViewport();
		DialogueWidgetInstance = nullptr;
		if (auto* PlayerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
			PlayerCharacter->bIsTalk = false;
			PlayerCharacter->SetCurrentTalkNPC(nullptr);
			CurrentIndex = 0;
		}
	}
}

void UDialogueComponent::SetDialogueCache(UDataTable* InDialogueTable)
{
	if (InDialogueTable) {
		NowDialogueTable = InDialogueTable;
		static const FString ContextString(TEXT("Dialogue Cache Context"));
		TArray<FDialogueTable*> DialogueRows;
		NowDialogueTable->GetAllRows<FDialogueTable>(ContextString, DialogueRows);
		for (FDialogueTable* DialogueRow : DialogueRows) {
			if (DialogueRow) {
				DialogueCache.Add(DialogueRow->TableID, DialogueRow);
			}
		}
	}
}

UDialogueWidget* UDialogueComponent::GetDialogueWidgetInstance()
{
	return DialogueWidgetInstance ? DialogueWidgetInstance : nullptr;
}

