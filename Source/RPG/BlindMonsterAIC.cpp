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
		HearingConfig->HearingRange = HearingRange; //청각 반경
		HearingConfig->SetMaxAge(5.f); //5초동안 청각정보 유지
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true; //적 탐지
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true; //아군 탐지
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true; //중립 탐지

		//인식 컴포넌트에 시각 인식 설정 적용
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
		//주된 탐지 감각은 시각으로
		GetPerceptionComponent()->SetDominantSense(*HearingConfig->GetSenseImplementation());
	}

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
	if (DamageConfig) {
		//인식 컴포넌트에 데미지 인식 설정 적용
		GetPerceptionComponent()->ConfigureSense(*DamageConfig);
	}

	//목표물 탐지 업데이트 이벤트
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABlindMonsterAIC::OnTargetDetected);
}

void ABlindMonsterAIC::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(Actor)) {
		GetBlackboardComponent()->SetValueAsBool(TEXT("CanDetectedPlayer"), Stimulus.WasSuccessfullySensed());
		//타겟 액터를 블랙보드에 저장
		if (Stimulus.WasSuccessfullySensed()) {
			GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), PlayerCharacter);
		}
		else {
			GetBlackboardComponent()->ClearValue(TEXT("TargetActor"));
		}
	}
}
