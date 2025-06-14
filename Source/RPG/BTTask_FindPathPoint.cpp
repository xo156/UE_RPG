// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "MonsterBase.h"
#include "PatrolPath.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint()
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
		return EBTNodeResult::Failed;

	auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
		return EBTNodeResult::Failed;

	//patrolpath(몬스터가 거쳐가는 위치 좌표) index를 블랙보드에서 가져오기
	int32 PatrolPathIndex = BlackboardComponent->GetValueAsInt(GetSelectedBlackboardKey());

	auto* MonsterBase = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!MonsterBase)
		return EBTNodeResult::Failed;
	
	//현재 monster가 가지고 있는 patrol path vector(patrol path actor에 있는)
	FVector LocalPoint = MonsterBase->GetPatrolPath()->GetPatrolPoints(PatrolPathIndex);
	auto GlobalPoint = MonsterBase->GetPatrolPath()->GetActorTransform().TransformPosition(LocalPoint);
	BlackboardComponent->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
