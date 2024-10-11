// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_BossMonsterAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTService_BossMonsterAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_BossMonsterAttackRange();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
