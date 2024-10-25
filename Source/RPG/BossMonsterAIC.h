// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossMonsterAIC.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ABossMonsterAIC : public AAIController
{
	GENERATED_BODY()
	
public:
	ABossMonsterAIC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
