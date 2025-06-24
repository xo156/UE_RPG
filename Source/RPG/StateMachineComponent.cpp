// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachineComponent.h"
#include "MonsterBase.h"

// Sets default values for this component's properties
UStateMachineComponent::UStateMachineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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

void UStateMachineComponent::SetPlayerState(EPlayerState NewState)
{
	if (CurrentPlayerState != NewState)
		CurrentPlayerState = NewState;
}

void UStateMachineComponent::SetMonsterState(EMonsterState NewState)
{
	if (CurrentMonsterState != NewState)
		CurrentMonsterState = NewState;

	/*auto* MonsterBase = Cast<AMonsterBase>(GetOwner());
	if (!MonsterBase)
		return;

	MonsterBase->OnExitState(CurrentMonsterState);

	EMonsterState OldState = CurrentMonsterState;
	CurrentMonsterState = NewState;

	MonsterBase->OnEnterState(NewState);*/
}

void UStateMachineComponent::SetNPCState(ENPCState NewState)
{
	if (CurrentNPCState != NewState)
		CurrentNPCState = NewState;
}

void UStateMachineComponent::SetAnimalState(EAnimalState NewState)
{
	if (CurrentAnimalState != NewState)
		CurrentAnimalState = NewState;
}
