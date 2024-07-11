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
//					// 블랙보드에서 읽어온 속도값에 따라 AI 캐릭터의 속도 설정
//					if (*SpeedValue == Monster->GetPatrolSpeed()) {
//						Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->GetPatrolSpeed();
//					}
//					else if (*SpeedValue == Monster->GetChaseSpeed()) {
//						Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->GetChaseSpeed();
//					}
//					else {
//						// 기본적으로 PatrolSpeed로 설정
//						Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->GetPatrolSpeed();
//					}
//				}
//			}
//		}
//	}
//}