// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeSpeed.h"
#include "MonsterAICSight.h"
#include "MonsterAICHearing.h"
#include "Monster.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	if (AMonsterAICHearing* MonsterAICHearing = Cast<AMonsterAICHearing>(OwnerComp.GetAIOwner())) {
		if (AMonster* Monster = Cast<AMonster>(MonsterAICHearing->GetPawn())) {
			Monster->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}