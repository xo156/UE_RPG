// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MonsterAICSight.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AMonsterAICSight : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterAICSight();

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

private:
	class UAISenseConfig_Sight* SightConfig; //½Ã°¢
};
