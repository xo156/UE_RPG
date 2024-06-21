// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_IncrementPatrolPath.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTTask_IncrementPatrolPath : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_IncrementPatrolPath();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	enum class EDirectionType {Forward, Reserve};

	EDirectionType Direction = EDirectionType::Forward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bBiDirectional = false; //앞으로? 뒤로? 에디터에서 설정하기
};
