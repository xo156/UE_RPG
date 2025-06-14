// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerIsInAttackRange.h"
#include "PatrolMonsterAIC.h"
#include "BlindMonsterAIC.h"
#include "MonsterBase.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerIsInAttackRange::UBTService_PlayerIsInAttackRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Attack Range");
}

void UBTService_PlayerIsInAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//¿œπ›
	if (bIsSight) {
		if (auto* PatrolMonsterAIC = Cast<APatrolMonsterAIC>(OwnerComp.GetAIOwner())) {
			if (auto* MonsterBase = Cast<AMonsterBase>(PatrolMonsterAIC->GetPawn())) {
				if (auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), MonsterBase->GetDistanceTo(PlayerCharacter) <= AttackRange);
				}
			}
		}
	}

	if (bIsHearing) {
		if (auto* BlindMonsterAIC = Cast<ABlindMonsterAIC>(OwnerComp.GetAIOwner())) {
			if (auto* MonsterBase = Cast<AMonsterBase>(BlindMonsterAIC->GetPawn())) {
				if (auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), MonsterBase->GetDistanceTo(PlayerCharacter) <= AttackRange);
				}
			}
		}
	}
}
