// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IncrementPatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "MonsterBase.h"
#include "PatrolPath.h"

UBTTask_IncrementPatrolPath::UBTTask_IncrementPatrolPath()
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncrementPatrolPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
		return EBTNodeResult::Failed;

	auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
		return EBTNodeResult::Failed;

	auto* MonsterBase = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!MonsterBase)
		return EBTNodeResult::Failed;

	int NumberOfPoints = MonsterBase->GetPatrolPath()->GetPatrolPointsNum();
	int MinIndex = 0;
	int MaxIndex = NumberOfPoints - 1;
	int32 Index = BlackboardComponent->GetValueAsInt(GetSelectedBlackboardKey());
	if (bBiDirectional) {
		//첫번째 혹은 마지막째가 되면 방향을 바꾸자
		if (Index >= MaxIndex && Direction == EDirectionType::Forward) {
			Direction = EDirectionType::Reserve;
		}
		else if (Index == MinIndex && Direction == EDirectionType::Reserve) {
			Direction = EDirectionType::Forward;
		}
	}
	BlackboardComponent->SetValueAsInt(GetSelectedBlackboardKey(), (Direction == EDirectionType::Forward ? ++Index : --Index) % NumberOfPoints);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
