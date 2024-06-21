// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_NormalMonsterAttack.h"
#include "NormalMonsterAIC.h"
#include "NormalMonster.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_NormalMonsterAttack::UBTTask_NormalMonsterAttack()
{
	NodeName = TEXT("NormalMonster Attack");
}

EBTNodeResult::Type UBTTask_NormalMonsterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//범위 밖이면 공격 안하도록
	if (bool bOutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey())) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	if (ANormalMonsterAIC* NormalMonsterAIC = Cast<ANormalMonsterAIC>(OwnerComp.GetAIOwner())) {
		if (ANormalMonster* NormalMonster = Cast<ANormalMonster>(NormalMonsterAIC->GetPawn())) {
			if (bMontageHasFinished(NormalMonster)) {
				NormalMonster->MonsterAttack();
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}

bool UBTTask_NormalMonsterAttack::bMontageHasFinished(ANormalMonster* NormalMonster)
{
	return NormalMonster->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(NormalMonster->GetNormalMonsterAttackMontage());
}
