// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalMonsterAIC.h"
#include "NormalMonster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "MyCharacter.h"

ANormalMonsterAIC::ANormalMonsterAIC()
{
	SetupPerceptionSystem();

}

void ANormalMonsterAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ANormalMonster* NormalMonster = Cast<ANormalMonster>(InPawn)) {
		if (UBehaviorTree* const BehaviorTree = NormalMonster->GetBehaviorTree()) {
			UBlackboardComponent* BB;
			UseBlackboard(BehaviorTree->BlackboardAsset, BB);
			Blackboard = BB;
			RunBehaviorTree(BehaviorTree);
		}
	}
}

void ANormalMonsterAIC::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig) {
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 500.f; //�þ� �ݰ�
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f; //�þ߿��� ����� �ݰ�
		SightConfig->PeripheralVisionAngleDegrees = 98.f; //�þ� ��
		SightConfig->SetMaxAge(5.f); //5�ʵ��� �þ����� ����
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f; //���������� �� ��ġ���� �ڵ�����
		SightConfig->DetectionByAffiliation.bDetectEnemies = true; //�� Ž��
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true; //�Ʊ� Ž��
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //�߸� Ž��

		//�ֵ� Ž�� ������ �ð�����
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		//��ǥ�� Ž�� ������Ʈ �̺�Ʈ
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANormalMonsterAIC::OnTargetDetected);
		//�ν� ������Ʈ�� �ð� �ν� ���� ����
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void ANormalMonsterAIC::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(Actor)) {
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}
