// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerIsInAttackRange.h"
#include "MonsterAICSight.h"
#include "MonsterAICHearing.h"
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
	if (bIsSight) {
		if (auto* MonsterAICSight = Cast<AMonsterAICSight>(OwnerComp.GetAIOwner())) {
			if (auto* Monster = Cast<AMonster>(MonsterAICSight->GetPawn())) {
				if (auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {
					AttackRange = Monster->GetAttackCollisionLength(Monster->GetAttackCollision());
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), Monster->GetDistanceTo(PlayerCharacter) <= AttackRange);
				}
			}
		}
	}

	if (bIsHearing) {
		if (auto* MonsterAICHearing = Cast<AMonsterAICHearing>(OwnerComp.GetAIOwner())) {
			if (auto* Monster = Cast<AMonster>(MonsterAICHearing->GetPawn())) {
				if (auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {
					AttackRange = Monster->GetAttackCollisionLength(Monster->GetAttackCollision());
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), Monster->GetDistanceTo(PlayerCharacter) <= AttackRange);
				}
			}
		}
	}
}
