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
		SightConfig->SightRadius = SightRadius; //시야 반경
		SightConfig->LoseSightRadius = LoseSightRadius; //시야에서 벗어나는 반경
		SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngle; //시야 각
		SightConfig->SetMaxAge(5.f); //시야정보 유지
		SightConfig->AutoSuccessRangeFromLastSeenLocation = SightRadius + 20.f;; //마지막으로 본 위치에서 자동으로
		SightConfig->DetectionByAffiliation.bDetectEnemies = true; //적 탐지
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true; //아군 탐지
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //중립 탐지
		
		//인식 컴포넌트에 시각 인식 설정 적용
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
		//주된 탐지 감각은 시각으로
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	}

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (HearingConfig) {
		HearingConfig->HearingRange = HearingRange; //청각 반경
		HearingConfig->SetMaxAge(3.f); //청각정보 유지
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true; //적 탐지
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true; //아군 탐지
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true; //중립 탐지

		//인식 컴포넌트에 청각 인식 설정 적용
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
	if (DamageConfig) {
		//인식 컴포넌트에 데미지 인식 설정 적용
		GetPerceptionComponent()->ConfigureSense(*DamageConfig);
	}

	//목표물 탐지 업데이트 이벤트
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &APatrolMonsterAIC::OnTargetDetected);
}

void APatrolMonsterAIC::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(Actor)) {
		GetBlackboardComponent()->SetValueAsBool("CanDetectedPlayer", Stimulus.WasSuccessfullySensed());
		//타겟 액터를 블랙보드에 저장
		if (Stimulus.WasSuccessfullySensed()) {
			GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerCharacter);
		}
		else {
			GetBlackboardComponent()->ClearValue("TargetActor");
		}
	}
}
