// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"
#include "NormalMonsterAIC.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	NodeName = TEXT("FindRandomLocation In NavMesh");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ANormalMonsterAIC* NormalMonsterAIC = Cast<ANormalMonsterAIC>(OwnerComp.GetAIOwner())) {
		if (APawn* NormalMonster = NormalMonsterAIC->GetPawn()) {
			if (!bInitOriginPosSet) { //처음에만 위치 정보를 저장
				OriginPos = NormalMonster->GetActorLocation();
				bInitOriginPosSet = true;
			}
			//FVector OriginPos = NormalMonster->GetActorLocation();
			if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld())) {
				FNavLocation NavLocation;
				if (NavSystem->GetRandomPointInNavigableRadius(OriginPos, SearchRadius, NavLocation)) {
					float Distance = FVector::Dist(OriginPos, NavLocation.Location);
					if (Distance <= MaxDistance) {
						OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavLocation.Location);
					}
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}
