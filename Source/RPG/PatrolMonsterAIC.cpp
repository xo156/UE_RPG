// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolMonsterAIC.h"
#include "MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "MyCharacter.h"

APatrolMonsterAIC::APatrolMonsterAIC()
{
	SetupPerceptionSystem();
}

void APatrolMonsterAIC::OnPossess(APawn* InPawn)
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

void APatrolMonsterAIC::SetupPerceptionSystem()
{
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig) {
		SightConfig->SightRadius = SightRadius; //�þ� �ݰ�
		SightConfig->LoseSightRadius = LoseSightRadius; //�þ߿��� ����� �ݰ�
		SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngle; //�þ� ��
		SightConfig->SetMaxAge(5.f); //�þ����� ����
		SightConfig->AutoSuccessRangeFromLastSeenLocation = SightRadius + 20.f;; //���������� �� ��ġ���� �ڵ�����
		SightConfig->DetectionByAffiliation.bDetectEnemies = true; //�� Ž��
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true; //�Ʊ� Ž��
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //�߸� Ž��
		
		//�ν� ������Ʈ�� �ð� �ν� ���� ����
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
		//�ֵ� Ž�� ������ �ð�����
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	}

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (HearingConfig) {
		HearingConfig->HearingRange = HearingRange; //û�� �ݰ�
		HearingConfig->SetMaxAge(3.f); //û������ ����
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true; //�� Ž��
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true; //�Ʊ� Ž��
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true; //�߸� Ž��

		//�ν� ������Ʈ�� û�� �ν� ���� ����
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
	if (DamageConfig) {
		//�ν� ������Ʈ�� ������ �ν� ���� ����
		GetPerceptionComponent()->ConfigureSense(*DamageConfig);
	}

	//��ǥ�� Ž�� ������Ʈ �̺�Ʈ
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &APatrolMonsterAIC::OnTargetDetected);
}

void APatrolMonsterAIC::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(Actor)) {
		GetBlackboardComponent()->SetValueAsBool("CanDetectedPlayer", Stimulus.WasSuccessfullySensed());
		//Ÿ�� ���͸� �����忡 ����
		if (Stimulus.WasSuccessfullySensed()) {
			GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerCharacter);
		}
		else {
			GetBlackboardComponent()->ClearValue("TargetActor");
		}
	}
}
