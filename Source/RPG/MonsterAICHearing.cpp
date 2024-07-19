// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAICHearing.h"
#include "Monster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "MyCharacter.h"

AMonsterAICHearing::AMonsterAICHearing()
{
	SetupPerceptionSystem();
}

void AMonsterAICHearing::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (auto* Monster = Cast<AMonster>(InPawn)) {
		if (UBehaviorTree* const BehaviorTree = Monster->GetBehaviorTree()) {
			UBlackboardComponent* BB;
			UseBlackboard(BehaviorTree->BlackboardAsset, BB);
			Blackboard = BB;
			RunBehaviorTree(BehaviorTree);
		}
	}
}

void AMonsterAICHearing::SetupPerceptionSystem()
{
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (HearingConfig) {
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		HearingConfig->HearingRange = 600.f; //û�� �ݰ�
		//HearingConfig->LoSHearingRange = HearingConfig->HearingRange + 50.f; //�Ҹ��� �ȵ鸮�ԵǴ� �ݰ�
		//HearingConfig->bUseLoSHearing = true; //�Ҹ��� �̿��� û�� Ž��
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true; //�� Ž��
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true; //�Ʊ� Ž��
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true; //�߸� Ž��

		//�ֵ� Ž�� ������ �ð�����
		GetPerceptionComponent()->SetDominantSense(*HearingConfig->GetSenseImplementation());
		//��ǥ�� Ž�� ������Ʈ �̺�Ʈ
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAICHearing::OnTargetDetected);
		//�ν� ������Ʈ�� �ð� �ν� ���� ����
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}
}

void AMonsterAICHearing::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(Actor)) {
		GetBlackboardComponent()->SetValueAsBool("CanHearPlayer", Stimulus.WasSuccessfullySensed());
		UE_LOG(LogTemp, Warning, TEXT("Detect Player Sound"));
	}
}
