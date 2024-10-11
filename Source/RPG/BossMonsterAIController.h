// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ABossMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABossMonsterAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
};
