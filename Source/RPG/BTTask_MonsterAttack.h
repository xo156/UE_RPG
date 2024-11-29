// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MonsterAttack.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTTask_MonsterAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_MonsterAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsSight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsHearing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsBoss = false;

	int32 PatternNumber;
};
