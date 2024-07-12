// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MonsterAICHearing.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AMonsterAICHearing : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterAICHearing();

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

private:
	class UAISenseConfig_Hearing* HearingConfig; //Ã»°¢
};
