// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonsterAIController.h"
#include "BossMonster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

ABossMonsterAIController::ABossMonsterAIController()
{

}

void ABossMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (auto* Monster = Cast<AMonster>(InPawn)) {
		if (UBehaviorTree* const BehaviorTree = Monster->GetBehaviorTree()) {
			UBlackboardComponent* BB;
			UseBlackboard(BehaviorTree->BlackboardAsset, BB);
			Blackboard = BB;
			RunBehaviorTree(BehaviorTree);
		}
	}
}
