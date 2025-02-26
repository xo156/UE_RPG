// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachineComponent.h"
#include "ResourceComponent.h"

// Sets default values for this component's properties
UStateMachineComponent::UStateMachineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	CurrentState = EPlayerState::PlayerIdle;
}


// Called when the game starts
void UStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStateMachineComponent::SetCurrentState(EPlayerState NewState)
{
	switch (NewState)
	{
	case EPlayerState::PlayerIdle:
		CurrentState = NewState;
		break;
	case EPlayerState::PlayerWalk:
		CurrentState = NewState;
		break;
	case EPlayerState::PlayerRun:
		CurrentState = NewState;
		break;
	case EPlayerState::PlayerJump:
		CurrentState = NewState;
		break;
	case EPlayerState::PlayerAttack:
		CurrentState = NewState;
		break;
	case EPlayerState::PlayerHit:
		CurrentState = NewState;
		break;
	default:
		CurrentState = EPlayerState::PlayerIdle;
		break;
	}
}

EPlayerState UStateMachineComponent::GetCurrentState()
{
	return CurrentState;
}

