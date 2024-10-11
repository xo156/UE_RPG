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
			//TODO:���� ���ͺ��� �ϼ��ϰ� �Ÿ��� ���� ���� ��Ÿ�� ���Ƴ����
			//��, ��, �������� ����
		}
	}

	return EBTNodeResult::Failed;
}
