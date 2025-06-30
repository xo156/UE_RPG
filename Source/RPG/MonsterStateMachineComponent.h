// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachineComponent.h"
#include "MonsterStateMachineComponent.generated.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Idle UMETA(DisplayName = "MonsterIdle"),
	Patrol UMETA(DisplayName = "MonsterPatrol"),
	Chase UMETA(DisplayName = "MonsterChase"),
	Attack UMETA(DisplayName = "MonsterAttack"),
	Hit UMETA(DisplayName = "MonsterHit"),
	Dead UMETA(DisplayName = "MonsterDead")
};

UCLASS()
class RPG_API UMonsterStateMachineComponent : public UStateMachineComponent
{
	GENERATED_BODY()

public:
	UMonsterStateMachineComponent();

	void ChangeState(EMonsterState NewState);

	EMonsterState GetCurrentState() const { return CurrentState; }
	EMonsterState GetPreviousState() const { return PreviousState; }

	bool IsInAnyState(const TArray<EMonsterState>& StatesToCheck) const;

protected:
	// Sets default values for this component's properties
	virtual void BeginPlay() override;
	virtual void OnTickState(float DeltaTime) override;

	void OnEnterState(EMonsterState State);
	void OnExitState(EMonsterState State);

private:
	EMonsterState CurrentState = EMonsterState::Idle;
	EMonsterState PreviousState = EMonsterState::Idle;

	float StateElapsedTime = 0.f;
};
