// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SelectAttackPattern.h"
#include "BTService_CheckPatternCondition.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "MonsterBase.h"

UBTTask_SelectAttackPattern::UBTTask_SelectAttackPattern()
{
	NodeName = TEXT("Select Attack Pattern");
}

EBTNodeResult::Type UBTTask_SelectAttackPattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
		return EBTNodeResult::Failed;

	auto* MonsterBase = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!MonsterBase)
		return EBTNodeResult::Failed;

	float TargetDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("TargetDistance"));
	float HPRatio = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("HPRatio"));
	MonsterBase->UpdateValidPatternIndexes(TargetDistance, HPRatio);

	const TArray<int32>& ValidIndexes = MonsterBase->GetValidPatternIndexes();
	if (ValidIndexes.Num() == 0) {
		return EBTNodeResult::Failed;
	}

	int32 SelectedIndex = ValidIndexes[FMath::RandRange(0, ValidIndexes.Num() - 1)];
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("SelectedPatternIndex"), SelectedIndex);

	return EBTNodeResult::Succeeded;
}
