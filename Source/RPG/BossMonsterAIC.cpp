// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonsterAIC.h"
#include "BossMonster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "MyCharacter.h"

ABossMonsterAIC::ABossMonsterAIC()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABossMonsterAIC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (auto* PlayerCharacter = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
		if (Blackboard) {
			float DistanceToPlayer = FVector::Dist(PlayerCharacter->GetActorLocation(), GetPawn()->GetActorLocation());
			if (DistanceToPlayer >= 800.f) {
				Blackboard->SetValueAsBool(FName("PlayerIsInChaseRange"), true);
			}
			else {
				Blackboard->SetValueAsBool(FName("PlayerIsInChaseRange"), false);
			}
		}
	}
}

void ABossMonsterAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (auto* BossMonster = Cast<ABossMonster>(InPawn)) {
		if (UBehaviorTree* const BehaviorTree = BossMonster->GetBehaviorTree()) {
			UBlackboardComponent* BB;
			UseBlackboard(BehaviorTree->BlackboardAsset, BB);
			Blackboard = BB;
			RunBehaviorTree(BehaviorTree);
		}
	}
}
