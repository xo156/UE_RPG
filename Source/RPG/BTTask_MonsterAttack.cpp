// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MonsterAttack.h"
#include "MonsterAICSight.h"
#include "MonsterAICHearing.h"
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

	if (bIsSight) {
		if (auto* MonsterAICSight = Cast<AMonsterAICSight>(OwnerComp.GetAIOwner())) {
			if (auto* Monster = Cast<AMonster>(MonsterAICSight->GetPawn())) {
				Monster->MonsterAttackStart();
				Monster->MonsterAttackExecute();
				FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
			}
		}
	}

	if (bIsHearing) {
		if (auto* MonsterAICHearing = Cast<AMonsterAICHearing>(OwnerComp.GetAIOwner())) {
			if (auto* Monster = Cast<AMonster>(MonsterAICHearing->GetPawn())) {
				Monster->MonsterAttackStart();
				Monster->MonsterAttackExecute();
				FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_MonsterAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	if (bIsSight) {
		if (auto* MonsterAICSight = Cast<AMonsterAICSight>(OwnerComp.GetAIOwner())) {
			if (auto* Monster = Cast<AMonster>(MonsterAICSight->GetPawn())) {
				if (bMontageHasFinished(Monster)) {
					Monster->MonsterAttackEnd();
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
			}
		}
	}

	if (bIsHearing) {
		if (auto* MonsterAICHearing = Cast<AMonsterAICHearing>(OwnerComp.GetAIOwner())) {
			if (auto* Monster = Cast<AMonster>(MonsterAICHearing->GetPawn())) {
				if (bMontageHasFinished(Monster)) {
					Monster->MonsterAttackEnd();
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
			}
		}
	}
}

bool UBTTask_MonsterAttack::bMontageHasFinished(AMonster* Monster)
{
	if (!Monster || !Monster->GetMesh()->GetAnimInstance()) {
		return false;
	}

	if (UAnimInstance* AnimInstance = Monster->GetMesh()->GetAnimInstance())
		return !AnimInstance->Montage_IsPlaying(Monster->GetMonsterAttackMontage());
	return false;
}