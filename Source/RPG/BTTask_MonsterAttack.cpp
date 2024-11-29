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
				if (!BossMonster->bIsMonsterAttack) {
					PatternNumber = OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());
					switch (PatternNumber) {
					case 1:
						BossMonster->MonsterAttackStart();
						BossMonster->MonsterAttackExecute(1);
						break;
					case 2:
						BossMonster->MonsterAttackStart();
						BossMonster->MonsterAttackExecute(2);
						break;
					case 3:
						BossMonster->MonsterAttackStart();
						BossMonster->MonsterAttackExecute(3);
						break;
					default:
						return EBTNodeResult::Succeeded;
						break;
					}
					return EBTNodeResult::InProgress;
				}
			}
		}
		return EBTNodeResult::Failed;
	}
	else {//보스 말고 일반
		//범위 밖이면 공격 안하도록
		if (bool bOutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey())) {
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}

		if (bIsSight) {
			if (auto* MonsterAICSight = Cast<AMonsterAICSight>(OwnerComp.GetAIOwner())) {
				if (auto* Monster = Cast<AMonster>(MonsterAICSight->GetPawn())) {
					if (Monster->bIsMonsterAttack)
						return EBTNodeResult::InProgress;
					Monster->MonsterAttackStart();
					Monster->MonsterAttackExecute();
					FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
				}
			}
		}

		if (bIsHearing) {
			if (auto* MonsterAICHearing = Cast<AMonsterAICHearing>(OwnerComp.GetAIOwner())) {
				if (auto* Monster = Cast<AMonster>(MonsterAICHearing->GetPawn())) {
					if (Monster->bIsMonsterAttack)
						return EBTNodeResult::InProgress;
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
				if (!Monster->bIsMonsterAttack)
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}

	if (bIsHearing) {
		if (auto* MonsterAICHearing = Cast<AMonsterAICHearing>(OwnerComp.GetAIOwner())) {
			if (auto* Monster = Cast<AMonster>(MonsterAICHearing->GetPawn())) {
				if (!Monster->bIsMonsterAttack)
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}

	if (bIsBoss) {
		if (auto* BossMonsterAIC = Cast<ABossMonsterAIC>(OwnerComp.GetAIOwner())) {
			if (auto* BossMonster = Cast<ABossMonster>(BossMonsterAIC->GetPawn())) {
				if (!BossMonster->bIsMonsterAttack)
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}
}
