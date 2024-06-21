// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeSpeed.h"
#include "NormalMonsterAIC.h"
#include "NormalMonster.h"
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

	if (ANormalMonsterAIC* NormalMonsterAIC = Cast<ANormalMonsterAIC>(OwnerComp.GetAIOwner())) {
		if (ANormalMonster* NormalMonster = Cast<ANormalMonster>(NormalMonsterAIC->GetPawn())) {
			NormalMonster->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}

//void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//{
//	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
//
//	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent()) {
//		if (float* SpeedValue = BlackboardComp->GetValue<UBlackboardKeyType_Float>(SpeedKey.GetSelectedKeyID())) {
//			if (ANormalMonsterAIC* NormalMonsterAIC = Cast<ANormalMonsterAIC>(OwnerComp.GetAIOwner())) {
//				if (ANormalMonster* NormalMonster = Cast<ANormalMonster>(NormalMonsterAIC->GetPawn())) {
//					// �����忡�� �о�� �ӵ����� ���� AI ĳ������ �ӵ� ����
//					if (*SpeedValue == NormalMonster->GetPatrolSpeed()) {
//						NormalMonster->GetCharacterMovement()->MaxWalkSpeed = NormalMonster->GetPatrolSpeed();
//					}
//					else if (*SpeedValue == NormalMonster->GetChaseSpeed()) {
//						NormalMonster->GetCharacterMovement()->MaxWalkSpeed = NormalMonster->GetChaseSpeed();
//					}
//					else {
//						// �⺻������ PatrolSpeed�� ����
//						NormalMonster->GetCharacterMovement()->MaxWalkSpeed = NormalMonster->GetPatrolSpeed();
//					}
//				}
//			}
//		}
//	}
//}