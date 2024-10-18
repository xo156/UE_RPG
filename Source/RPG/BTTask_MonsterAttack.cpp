// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MonsterAttack.h"
#include "MonsterAICSight.h"
#include "MonsterAICHearing.h"
#include "Monster.h"
#include "BossMonsterAIC.h"
#include "BossMonster.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MonsterAttack::UBTTask_MonsterAttack()
{
	bNotifyTick = true;
	NodeName = TEXT("Monster Attack");
}

EBTNodeResult::Type UBTTask_MonsterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (bIsBoss) {
		if (auto* BossMonsterAIC = Cast<ABossMonsterAIC>(OwnerComp.GetAIOwner())) {
			if (auto* BossMonster = Cast<ABossMonster>(BossMonsterAIC->GetPawn())) {
				PatternNumber = OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());
				switch (PatternNumber) {
				case 1:
					BossMonster->MonsterAttackStart();
					BossMonster->MonsterAttackExecute(1);
					FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
					break;
				case 2:
					BossMonster->MonsterAttackStart();
					BossMonster->MonsterAttackExecute(2);
					FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
					break;
				case 3:
					BossMonster->MonsterAttackStart();
					BossMonster->MonsterAttackExecute(3);
					FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
					break;
				default:
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
					break;
				}
			}
		}
		return EBTNodeResult::Failed;
	}
	else {
		//���� ���̸� ���� ���ϵ���
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

	if (bIsBoss) {
		if (auto* BossMonsterAIC = Cast<ABossMonsterAIC>(OwnerComp.GetAIOwner())) {
			if (auto* BossMonster = Cast<ABossMonster>(BossMonsterAIC->GetPawn())) {
				if (bMontageHasFinished(BossMonster)) {
					BossMonster->MonsterAttackEnd();
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