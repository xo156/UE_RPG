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
			CurrentIndex = 0; //�ε����� 0����
			DialogueWidgetInstance->SetDialogueText(Dialogues[CurrentIndex]->Content);
			DialogueWidgetInstance->AddToViewport();
			DialogueWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UDialogueComponent::LoadDialogues(UDataTable* InDialogueTable)
{
	//�̰� DataTableGameInstance���� ĳ�� �ߴ���ó�� �ϱ�
	//Dialogues�� ��� �þ�� ���� ����
	if (InDialogueTable) {
		InDialogueTable->GetAllRows<FDialogueTable>(TEXT("Dialogues Context String"), Dialogues);
	}
}

void UDialogueComponent::NextDialogue()
{
	if (DialogueWidgetInstance) { //�̹� ������ �����Ѵٸ�
		CurrentIndex++; //���� ��ȭ�� ���� �ε��� ����
		if (CurrentIndex < Dialogues.Num()) { //���� �ִ� ��ȭ ���� �Ѿ�� �ʴ´ٸ�
			DialogueWidgetInstance->SetDialogueText(Dialogues[CurrentIndex]->Content); //���� ��ȭ ���			

			UE_LOG(LogTemp, Log, TEXT("CurrentIndex: %d"), CurrentIndex);
		}
		else { //�ִ� ��ȭ ���� �Ѿ�� ����
			DialogueWidgetInstance->RemoveFromViewport();
			DialogueWidgetInstance = nullptr;
			if (auto* PlayerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
				PlayerCharacter->bIsTalk = false; //��ȭ ���� ���� ����
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

