// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MonsterAttack.h"
#include "MonsterAICSight.h"
#include "Monster.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MonsterAttack::UBTTask_MonsterAttack()
{
	NodeName = TEXT("Monster Attack");
}

EBTNodeResult::Type UBTTask_MonsterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//범위 밖이면 공격 안하도록
	if (bool bOutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey())) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	if (auto* MonsterAICSight = Cast<AMonsterAICSight>(OwnerComp.GetAIOwner())) {
		if (auto* Monster = Cast<AMonster>(MonsterAICSight->GetPawn())) {
			if (bMontageHasFinished(Monster)) {
				Monster->MonsterAttack();
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}

bool UBTTask_MonsterAttack::bMontageHasFinished(AMonster* Monster)
{
	return Monster->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Monster->GetMonsterAttackMontage());
}
