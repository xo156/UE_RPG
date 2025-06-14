// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "MonsterBase.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	NodeName = TEXT("FindRandomLocation In NavMesh");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (!bInitOriginPosSet) { 
		//스폰시 위치 정보를 저장
		OriginPos = MonsterBase->GetActorLocation();
		bInitOriginPosSet = true;
	}
	auto* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem)
		return EBTNodeResult::Failed;

	FNavLocation NavLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(OriginPos, SearchRadius, NavLocation)) {
		float Distance = FVector::Dist(OriginPos, NavLocation.Location);
		if (Distance <= MaxDistance) {
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavLocation.Location);
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
