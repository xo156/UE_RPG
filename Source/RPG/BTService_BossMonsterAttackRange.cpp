// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BossMonsterAttackRange.h"
#include "BossMonsterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BossMonster.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_BossMonsterAttackRange::UBTService_BossMonsterAttackRange()
{
	NodeName = TEXT("BossMonsterAttackRange");
}

void UBTService_BossMonsterAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (auto* BossMonsterAIController = OwnerComp.GetAIOwner()) {
		if (auto* BossMonster = Cast<ABossMonster>(BossMonsterAIController->GetOwner())) {
			if (auto* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) {
				float DistanceToPlayer = FVector::Dist(PlayerPawn->GetActorLocation(), BossMonster->GetActorLocation());
				OwnerComp.GetBlackboardComponent()->SetValueAsFloat("DistanceToPlayer", DistanceToPlayer);
				OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", PlayerPawn->GetActorLocation());
			}
		}
	}
}


