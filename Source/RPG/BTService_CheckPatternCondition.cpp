// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckPatternCondition.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "MonsterBase.h"
#include "HPActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterAttackPatternDataAsset.h"

UBTService_CheckPatternCondition::UBTService_CheckPatternCondition()
{
	NodeName = TEXT("Check Attack Pattern Condition");
	bNotifyTick = true;

	TargetActorKey.SelectedKeyName = "TargetActor";
	TargetDistanceKey.SelectedKeyName = "TargetDistance";
	HPRatioKey.SelectedKeyName = "HPRatio";
}

void UBTService_CheckPatternCondition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!OwnerComp.GetAIOwner())
		return;

	auto* MonsterBase = Cast<AMonsterBase>(OwnerComp.GetAIOwner());
	if (!MonsterBase) 
		return;

	auto* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor)
		return;

	float Distance = MonsterBase->GetDistanceTo(TargetActor);
	float HPRatio = MonsterBase->GetHPActorComponent()->GetHPRatio();

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TargetDistanceKey.SelectedKeyName, Distance);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(HPRatioKey.SelectedKeyName, HPRatio);
}
