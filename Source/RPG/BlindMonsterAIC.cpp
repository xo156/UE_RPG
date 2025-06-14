// Fill out your copyright notice in the Description page of Project Settings.


#include "BlindMonsterAIC.h"
#include "MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "MyCharacter.h"

ABlindMonsterAIC::ABlindMonsterAIC()
{
	SetupPerceptionSystem();
}

void ABlindMonsterAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (auto* Monster = Cast<AMonsterBase>(InPawn)) {
		if (UBehaviorTree* const BehaviorTree = Monster->GetBehaviorTree()) {
			UBlackboardComponent* BB;
			UseBlackboard(BehaviorTree->BlackboardAsset, BB);
			Blackboard = BB;
			RunBehaviorTree(BehaviorTree);
		}
	}
}

void ABlindMonsterAIC::SetupPerceptionSystem()
{
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (HearingConfig) {
		HearingConfig->HearingRange = HearingRange; //û�� �ݰ�
		HearingConfig->SetMaxAge(5.f); //5�ʵ��� û������ ����
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true; //�� Ž��
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true; //�Ʊ� Ž��
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true; //�߸� Ž��

		//�ν� ������Ʈ�� �ð� �ν� ���� ����
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
		//�ֵ� Ž�� ������ �ð�����
		GetPerceptionComponent()->SetDominantSense(*HearingConfig->GetSenseImplementation());
	}

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
	if (DamageConfig) {
		//�ν� ������Ʈ�� ������ �ν� ���� ����
		GetPerceptionComponent()->ConfigureSense(*DamageConfig);
	}

	//��ǥ�� Ž�� ������Ʈ �̺�Ʈ
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABlindMonsterAIC::OnTargetDetected);
}

void ABlindMonsterAIC::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(Actor)) {
		GetBlackboardComponent()->SetValueAsBool(TEXT("CanDetectedPlayer"), Stimulus.WasSuccessfullySensed());
		//Ÿ�� ���͸� �����忡 ����
		if (Stimulus.WasSuccessfullySensed()) {
			GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), PlayerCharacter);
		}
		else {
			GetBlackboardComponent()->ClearValue(TEXT("TargetActor"));
		}
	}
}
