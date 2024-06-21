// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerIsInAttackRange.h"
#include "NormalMonsterAIC.h"
#include "NormalMonster.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerIsInAttackRange::UBTService_PlayerIsInAttackRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Attack Range");
}

void UBTService_PlayerIsInAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ANormalMonsterAIC* NormalMonsterAIC = Cast<ANormalMonsterAIC>(OwnerComp.GetAIOwner())) {
		if (ANormalMonster* NormalMonster = Cast<ANormalMonster>(NormalMonsterAIC->GetPawn())) {
			if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), NormalMonster->GetDistanceTo(PlayerCharacter) <= AttackRange);
			}
		}
	}
}
