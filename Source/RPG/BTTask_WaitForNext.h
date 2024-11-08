// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_WaitForNext.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTTask_WaitForNext : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_WaitForNext();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void SetAroundLocation(AActor* PlayerCharacter, class ABossMonster* BossMonster);

	float ElapsedTime;
	float WaitForNextActionTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MinAroundRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MaxAroundRadius;

	FVector AroundLocation;
};
