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
			CurrentIndex = 0; //인덱스를 0으로
			DialogueWidgetInstance->SetDialogueText(Dialogues[CurrentIndex]->Content);
			DialogueWidgetInstance->AddToViewport();
			DialogueWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UDialogueComponent::LoadDialogues(UDataTable* InDialogueTable)
{
	//이거 DataTableGameInstance에서 캐시 했던거처럼 하기
	//Dialogues가 계속 늘어나느 문제 있음
	if (InDialogueTable) {
		InDialogueTable->GetAllRows<FDialogueTable>(TEXT("Dialogues Context String"), Dialogues);
	}
}

void UDialogueComponent::NextDialogue()
{
	if (DialogueWidgetInstance) { //이미 위젯이 존재한다면
		CurrentIndex++; //다음 대화를 위해 인덱스 증가
		if (CurrentIndex < Dialogues.Num()) { //만약 최대 대화 수를 넘어서지 않는다면
			DialogueWidgetInstance->SetDialogueText(Dialogues[CurrentIndex]->Content); //다음 대화 출력			

			UE_LOG(LogTemp, Log, TEXT("CurrentIndex: %d"), CurrentIndex);
		}
		else { //최대 대화 수를 넘어서면 제거
			DialogueWidgetInstance->RemoveFromViewport();
			DialogueWidgetInstance = nullptr;
			if (auto* PlayerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
				PlayerCharacter->bIsTalk = false; //대화 종료 상태 설정
				PlayerCharacter->SetCurrentTalkNPC(nullptr);
			}
			CurrentIndex = 0;
			dia
		}
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

