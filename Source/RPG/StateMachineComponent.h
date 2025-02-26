// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachineComponent.generated.h"


UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	PlayerIdle UMETA(DisplayName = "PlayerIdle"),
	PlayerWalk UMETA(DisplayName = "PlayerWalk"),
	PlayerRun UMETA(DisplayName = "PlayerRun"),
	PlayerJump UMETA(DisplayName = "PlayerJump"),
	PlayerAttack UMETA(DisplayName = "PlayerAttack"),
	PlayerHit UMETA(DisplayName = "PlayerHit"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateMachineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetCurrentState(EPlayerState NewState);
	EPlayerState GetCurrentState();

private:
	EPlayerState CurrentState;
};
