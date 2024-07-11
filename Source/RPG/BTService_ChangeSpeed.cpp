// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeSpeed.h"
#include "MonsterAICSight.h"
#include "Monster.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "BehaviorTree/BlackboardComponent.h"
//#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (AMonsterAICSight* MonsterAICSight = Cast<AMonsterAICSight>(OwnerComp.GetAIOwner())) {
		if (AMonster* Monster = Cast<AMonster>(MonsterAICSight->GetPawn())) {
			Monster->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}

//void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//{
//	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
//
//	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent()) {
//		if (float* SpeedValue = BlackboardComp->GetValue<UBlackboardKeyType_Float>(SpeedKey.GetSelectedKeyID())) {
//			if (AMonsterAICSight* MonsterAICSight = Cast<AMonsterAICSight>(OwnerComp.GetAIOwner())) {
//				if (AMonster* Monster = Cast<AMonster>(MonsterAICSight->GetPawn())) {
//					// �����忡�� �о�� �ӵ����� ���� AI ĳ������ �ӵ� ����
//					if (*SpeedValue == Monster->GetPatrolSpeed()) {
//						Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->GetPatrolSpeed();
//					}
//					else if (*SpeedValue == Monster->GetChaseSpeed()) {
//						Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->GetChaseSpeed();
//					}
//					else {
//						// �⺻������ PatrolSpeed�� ����
//						Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->GetPatrolSpeed();
//					}
//				}
//			}
//		}
//	}
//}