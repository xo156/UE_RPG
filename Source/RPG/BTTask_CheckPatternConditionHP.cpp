// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckPatternConditionHP.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterBase.h"
#include "ResourceComponent.h"

UBTTask_CheckPatternConditionHP::UBTTask_CheckPatternConditionHP()
{
	NodeName = TEXT("Check Pattern Condition HP");

}

EBTNodeResult::Type UBTTask_CheckPatternConditionHP::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
		return EBTNodeResult::Failed;

	auto* MonsterBase = Cast<AMonsterBase>(OwnerComp.GetAIOwner());
	if (!MonsterBase)
		return EBTNodeResult::Failed;

	float HPRatio = MonsterBase->GetResourceComponent()->GetHPRatio();
	//OwnerComp.GetBlackboardComponent()->SetValueAsFloat("HPRatio", HPRatio);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(HPRatioKey.SelectedKeyName, HPRatio);

	return EBTNodeResult::Succeeded;
}
