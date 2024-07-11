// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IncrementPatrolPath.h"
#include "MonsterAICSight.h"
#include "Monster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrolPath.h"


UBTTask_IncrementPatrolPath::UBTTask_IncrementPatrolPath()
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncrementPatrolPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* MonsterAICSight = Cast<AMonsterAICSight>(OwnerComp.GetAIOwner())) {
		if (auto* Monster = Cast<AMonster>(MonsterAICSight->GetPawn())) {
			if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent()) {
				int NumberOfPoints = Monster->GetPatrolPath()->GetPatrolPointsNum();
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
		}
	}
	return EBTNodeResult::Failed;
}
