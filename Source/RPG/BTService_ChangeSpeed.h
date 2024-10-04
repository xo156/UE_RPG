// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTService_ChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_ChangeSpeed();

	virtual void OnBecomeRelevant(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Speed = 500.f; //나중에는 Monster에서 가져오는걸로 하자

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsSight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsHearing = false;
};
