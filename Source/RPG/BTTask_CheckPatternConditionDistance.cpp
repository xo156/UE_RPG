// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckPatternConditionDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterBase.h"

UBTTask_CheckPatternConditionDistance::UBTTask_CheckPatternConditionDistance()
{
	NodeName = TEXT("Check Pattern Condition Distance");
}

EBTNodeResult::Type UBTTask_CheckPatternConditionDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
		return EBTNodeResult::Failed;

	auto* MonsterBase = Cast<AMonsterBase>(OwnerComp.GetAIOwner());
	if (!MonsterBase)
		return EBTNodeResult::Failed;

	auto* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	if (!TargetActor)
		return EBTNodeResult::Failed;

	float Distance = MonsterBase->GetDistanceTo(TargetActor);
	//OwnerComp.GetBlackboardComponent()->SetValueAsFloat("TargetDistance", Distance);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TargetDistanceKey.SelectedKeyName, Distance);

	return EBTNodeResult::Succeeded;
}
