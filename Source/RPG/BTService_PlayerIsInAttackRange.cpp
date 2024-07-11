// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerIsInAttackRange.h"
#include "MonsterAICSight.h"
#include "Monster.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerIsInAttackRange::UBTService_PlayerIsInAttackRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Attack Range");
}

void UBTService_PlayerIsInAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* MonsterAICSight = Cast<AMonsterAICSight>(OwnerComp.GetAIOwner())) {
		if (auto* Monster = Cast<AMonster>(MonsterAICSight->GetPawn())) {
			if (auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), Monster->GetDistanceTo(PlayerCharacter) <= AttackRange);
			}
		}
	}
}
