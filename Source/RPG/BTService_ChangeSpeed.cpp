// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeSpeed.h"
#include "AIController.h"
#include "MonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
		return;

	auto* MonsterBase = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!MonsterBase)
		return;

	MonsterBase->GetCharacterMovement()->MaxWalkSpeed = MonsterBase->GetCharacterMovement()->MaxWalkSpeed * 1.5f;

}