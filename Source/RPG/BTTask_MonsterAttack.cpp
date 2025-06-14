// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MonsterAttack.h"
#include "MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterAttackPatternDataAsset.h"
#include "AIController.h"

UBTTask_MonsterAttack::UBTTask_MonsterAttack()
{
	NodeName = TEXT("Monster Attack");
}

EBTNodeResult::Type UBTTask_MonsterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
		return EBTNodeResult::Failed;

	auto* MonsterBase = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!MonsterBase) 
		return EBTNodeResult::Failed;

	int32 PatternIndex = BlackboardComponent->GetValueAsInt(TEXT("SelectedPatternIndex"));
	if (!MonsterBase->GetAttackPatterns().IsValidIndex(PatternIndex))
		return EBTNodeResult::Failed;

	MonsterBase->MonsterAttackStart(PatternIndex);

	return EBTNodeResult::Succeeded;
}
