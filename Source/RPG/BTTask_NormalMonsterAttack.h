// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_NormalMonsterAttack.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTTask_NormalMonsterAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_NormalMonsterAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool bMontageHasFinished(class ANormalMonster* NormalMonster);
};
