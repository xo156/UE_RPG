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
		HearingConfig->HearingRange = 600.f; //청각 반경
		//HearingConfig->LoSHearingRange = HearingConfig->HearingRange + 50.f; //소리가 안들리게되는 반경
		//HearingConfig->bUseLoSHearing = true; //소리를 이용한 청각 탐지
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true; //적 탐지
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true; //아군 탐지
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true; //중립 탐지

		//주된 탐지 감각은 시각으로
		GetPerceptionComponent()->SetDominantSense(*HearingConfig->GetSenseImplementation());
		//목표물 탐지 업데이트 이벤트
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAICHearing::OnTargetDetected);
		//인식 컴포넌트에 시각 인식 설정 적용
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
