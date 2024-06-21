// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathPoint.h"
#include "NormalMonsterAIC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NormalMonster.h"
#include "PatrolPath.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint()
{
	NodeName = TEXT("Find Path Point");
	//몬스터가 캐릭터를 찾지 못해서 돌아다니는 상황에 task
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ANormalMonsterAIC* NormalMonsterAIC = Cast<ANormalMonsterAIC>(OwnerComp.GetAIOwner())) {
		if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent()) {
			//patrolpath(몬스터가 거쳐가는 위치 좌표) index를 블랙보드에서 가져오기
			int32 Index = BlackboardComponent->GetValueAsInt(GetSelectedBlackboardKey());
			if (ANormalMonster* NormalMonster = Cast<ANormalMonster>(NormalMonsterAIC->GetPawn())) {
				//현재 normal monster가 가지고 있는 patrol path vector(patrol path actor에 있는)
				FVector LocalPoint = NormalMonster->GetPatrolPath()->GetPatrolPoints(Index);
				//이거 auto로 하는게 좋나?
				UE::Math::TVector<double> GlobalPoint = NormalMonster->GetPatrolPath()->GetActorTransform().TransformPosition(LocalPoint);
				BlackboardComponent->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
