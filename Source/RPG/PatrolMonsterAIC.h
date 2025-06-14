// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "PatrolMonsterAIC.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API APatrolMonsterAIC : public AAIController
{
	GENERATED_BODY()

public:
	APatrolMonsterAIC();

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

private:
	class UAISenseConfig_Sight* SightConfig; //시각
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIPerception|Sight", meta = (AllowPrivateAccess = "true"))
	float SightRadius = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIPerception|Sight", meta = (AllowPrivateAccess = "true"))
	float LoseSightRadius = 1200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIPerception|Sight", meta = (AllowPrivateAccess = "true"))
	float PeripheralVisionAngle = 90.f;

	class UAISenseConfig_Hearing* HearingConfig; //청각
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIPerception|Hearing", meta = (AllowPrivateAccess = "true"))
	float HearingRange = 600.f;

	class UAISenseConfig_Damage* DamageConfig; //충격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIPerception|Damage", meta = (AllowPrivateAccess = "true"))
	float DamageSenseMaxAge = 5.f;
};
