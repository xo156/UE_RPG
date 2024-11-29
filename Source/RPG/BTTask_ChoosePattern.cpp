// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChoosePattern.h"
#include "BossMonsterAIC.h"
#include "BossMonster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyCharacter.h"

UBTTask_ChoosePattern::UBTTask_ChoosePattern()
{
	NodeName = TEXT("Choose Pattern");
}

EBTNodeResult::Type UBTTask_ChoosePattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* BossMonsterAIC = Cast<ABossMonsterAIC>(OwnerComp.GetAIOwner())) {
		if (auto* BossMonster = Cast<ABossMonster>(BossMonsterAIC->GetPawn())) {
			if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent()) {
				PlayerLocation = BlackboardComponent->GetValueAsVector(FName("TargetLocation"));
				DistanceToPlayer = FVector::Dist(BossMonster->GetActorLocation(), PlayerLocation);
				RandomValue = FMath::RandRange(0.f, 1.f);
				if (DistanceToPlayer <= BossMonster->GetCloseRange()) {
					if (RandomValue < 0.8f) {
						//근거리
						OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), 1);
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
					else if (RandomValue < 0.9f) {
						//중거리
						OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), 2);
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
					else {
						//원거리
						OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), 3);
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
				}
				else if (DistanceToPlayer <= BossMonster->GetMidRange()) {
					if (RandomValue < 0.6f) {
						//중거리
						OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), 2);
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
					else {
						//원거리
						OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), 3);
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
				}
				else {
					if (RandomValue < 0.3f) {
						//중거리
						OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), 2);
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
					else {
						//원거리
						OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), 3);
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}
