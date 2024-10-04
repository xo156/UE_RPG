// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"
#include "Monster.h"
#include "MonsterAICSight.h"
#include "MonsterAICHearing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer()
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	FVector GoalLocation = PlayerLocation + FVector(0.f, -30.f, 0.f);

	if (bIsSight) {
		if (auto* MonsterAICSight = Cast<AMonsterAICSight>(OwnerComp.GetAIOwner())) {
			if (auto* Monster = Cast<AMonster>(MonsterAICSight->GetPawn())) {
				if (Monster->bIsMonsterAttack) {
					return EBTNodeResult::Succeeded;
				}
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(MonsterAICSight, GoalLocation);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	if (bIsHearing) {
		if (auto* MonsterAICHearing = Cast<AMonsterAICHearing>(OwnerComp.GetAIOwner())) {
			if (auto* Monster = Cast<AMonster>(MonsterAICHearing->GetPawn())) {
				if (Monster->bIsMonsterAttack) {
					return EBTNodeResult::Succeeded;
				}
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(MonsterAICHearing, GoalLocation);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
