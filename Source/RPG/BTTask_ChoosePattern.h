// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ChoosePattern.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTTask_ChoosePattern : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_ChoosePattern();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FVector PlayerLocation;
	float DistanceToPlayer;
	float RandomValue;
};
