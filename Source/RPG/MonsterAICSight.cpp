// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAICSight.h"
#include "Monster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MyCharacter.h"

AMonsterAICSight::AMonsterAICSight()
{
	SetupPerceptionSystem();

}

void AMonsterAICSight::OnPossess(APawn* InPawn)
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

void AMonsterAICSight::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig) {
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 500.f; //시야 반경
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f; //시야에서 벗어나는 반경
		SightConfig->PeripheralVisionAngleDegrees = 98.f; //시야 각
		SightConfig->SetMaxAge(5.f); //5초동안 시야정보 유지
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f; //마지막으로 본 위치에서 자동으로
		SightConfig->DetectionByAffiliation.bDetectEnemies = true; //적 탐지
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true; //아군 탐지
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //중립 탐지

		//주된 탐지 감각은 시각으로
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		//목표물 탐지 업데이트 이벤트
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAICSight::OnTargetDetected);
		//인식 컴포넌트에 시각 인식 설정 적용
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AMonsterAICSight::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(Actor)) {
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}
