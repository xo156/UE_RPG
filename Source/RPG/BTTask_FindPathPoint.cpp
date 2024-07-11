// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathPoint.h"
#include "MonsterAICSight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster.h"
#include "PatrolPath.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint()
{
	NodeName = TEXT("Find Path Point");
	//���Ͱ� ĳ���͸� ã�� ���ؼ� ���ƴٴϴ� ��Ȳ�� task
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* MonsterAICSight = Cast<AMonsterAICSight>(OwnerComp.GetAIOwner())) {
		if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent()) {
			//patrolpath(���Ͱ� ���İ��� ��ġ ��ǥ) index�� �����忡�� ��������
			int32 Index = BlackboardComponent->GetValueAsInt(GetSelectedBlackboardKey());
			if (auto* Monster = Cast<AMonster>(MonsterAICSight->GetPawn())) {
				//���� normal monster�� ������ �ִ� patrol path vector(patrol path actor�� �ִ�)
				FVector LocalPoint = Monster->GetPatrolPath()->GetPatrolPoints(Index);
				auto GlobalPoint = Monster->GetPatrolPath()->GetActorTransform().TransformPosition(LocalPoint);
				BlackboardComponent->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
