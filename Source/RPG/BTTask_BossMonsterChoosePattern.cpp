// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossMonsterChoosePattern.h"
#include "AIController.h"
#include "BossMonster.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_BossMonsterChoosePattern::UBTTask_BossMonsterChoosePattern()
{
	NodeName = TEXT("BossMonster Choose Attack Pattern");
}

EBTNodeResult::Type UBTTask_BossMonsterChoosePattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* BossMonsterAIController = OwnerComp.GetAIOwner()) {
		if (auto* BossMonster = Cast<ABossMonster>(BossMonsterAIController->GetOwner())) {
			float DistanceToPlayer = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("DistanceToPlayer");
			if (BossMonster->WanderCount < 2 && !BossMonster->bIsWander) {
				if (DistanceToPlayer > BossMonster->LongAttackRange) {
					//��å?
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
				else {
					if (DistanceToPlayer <= BossMonster->CloseAttackRange) {
						//�ٰŸ�
						BossMonster->MonsterAttackStart();
						BossMonster->MonsterAttackExecute(BossMonster->GetCloseAttackMontage());
					}
					else if (DistanceToPlayer <= BossMonster->MidAttackRange) {
						//�߰Ÿ�
						BossMonster->MonsterAttackStart();
						BossMonster->MonsterAttackExecute(BossMonster->GetMidAttackMontage());
					}
					else if (DistanceToPlayer <= BossMonster->LongAttackRange) {
						//���Ÿ�
						BossMonster->MonsterAttackStart();
						BossMonster->MonsterAttackExecute(BossMonster->GetLongAttackMontage());
					}
					//���� ���� ��ȸ�ϱ�
					BossMonster->FindPlayerAround();
					FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
					return EBTNodeResult::InProgress;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_BossMonsterChoosePattern::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	if (auto* BossMonsterAIController = OwnerComp.GetAIOwner()) {
		if (auto* BossMonster = Cast<ABossMonster>(BossMonsterAIController->GetOwner())) {
			if (bMontageHasFinished(BossMonster)) {
				BossMonster->MonsterAttackEnd();
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}
}

bool UBTTask_BossMonsterChoosePattern::bMontageHasFinished(ABossMonster* BossMonster)
{
	if (!BossMonster || !BossMonster->GetMesh()->GetAnimInstance())
		return false;

	if(auto* AnimInstance = BossMonster->GetMesh()->GetAnimInstance())
		return !AnimInstance->Montage_IsPlaying(BossMonster->GetNowAttackMontage());

	return false;
}
