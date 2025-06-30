// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachineComponent.h"
#include "PlayerStateMachineComponent.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle UMETA(DisplayName = "PlayerIdle"),
	Move UMETA(DisplayName = "PlayerMove"),
	LightAttack UMETA(DisplayName = "PlayerLightAttack"),
	HeavyAttack UMETA(DisplayName = "PlayerHeavyAttack"),
	Guard UMETA(DisplayName = "PlayerGuard"),
	Dodge UMETA(DisplayName = "PlayerDodge"),
	Parry UMETA(DisplayName = "PlayerParry"),
	Hit UMETA(DisplayName = "PlayerHit"),
	Dead UMETA(DisplayName = "PlayerDead")
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UPlayerStateMachineComponent : public UStateMachineComponent
{
	GENERATED_BODY()
	
public:
	UPlayerStateMachineComponent();

	void ChangeState(EPlayerState NewState);

	EPlayerState GetCurrentState() const { return CurrentState; }
	EPlayerState GetPreviousState() const { return PreviousState; }

	bool IsInAnyState(const TArray<EPlayerState>& StatesToCheck) const;

protected:
	// Sets default values for this component's properties
	virtual void BeginPlay() override;
	virtual void OnTickState(float DeltaTime) override;

	void OnEnterState(EPlayerState State);
	void OnExitState(EPlayerState State);

private:
	EPlayerState CurrentState = EPlayerState::Idle;
	EPlayerState PreviousState = EPlayerState::Idle;

	float StateElapsedTime = 0.f;
};
