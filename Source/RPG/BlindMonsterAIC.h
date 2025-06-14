// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BlindMonsterAIC.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ABlindMonsterAIC : public AAIController
{
	GENERATED_BODY()

public:
	ABlindMonsterAIC();

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

private:
	class UAISenseConfig_Hearing* HearingConfig; //청각
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIPerception|Hearing", meta = (AllowPrivateAccess = "true"))
	float HearingRange = 600.f;

	class UAISenseConfig_Damage* DamageConfig; //충격
};
