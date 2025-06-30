// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "MonsterBase.h"
#include "MonsterStateMachineComponent.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer()
{
	NodeName = TEXT("Chase Player");

	TargetActorKey.SelectedKeyName = "TargetActor";
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
		return EBTNodeResult::Failed;

	auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
		return EBTNodeResult::Failed;

	auto* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor)
		return EBTNodeResult::Failed;

	//FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	FVector PlayerLocation = TargetActor->GetActorLocation();
	FVector GoalLocation = PlayerLocation + FVector(0.f, -30.f, 0.f);

	auto* MonsterBase = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!MonsterBase)
		return EBTNodeResult::Failed;

	if (MonsterBase->GetMonsterStateMachineComponent()->IsInAnyState({EMonsterState::Attack})) {
		return EBTNodeResult::Succeeded;
	}
	else {
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(MonsterBase->GetInstigatorController(), GoalLocation);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
