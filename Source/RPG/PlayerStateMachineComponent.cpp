// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateMachineComponent.h"
#include "MonsterStateMachineComponent.h"

UPlayerStateMachineComponent::UPlayerStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentState = EPlayerState::Idle;
}

void UPlayerStateMachineComponent::BeginPlay() 
{
	Super::BeginPlay();
	OnEnterState(CurrentState);
}

void UPlayerStateMachineComponent::OnTickState(float DeltaTime)
{
	//�ǰݽ� �Ͻ� ���� ���� �ð� üũ
	if (CurrentState == EPlayerState::Hit && StateElapsedTime >= 1.0f) {
		ChangeState(EPlayerState::Idle);
	}
}

void UPlayerStateMachineComponent::ChangeState(EPlayerState NewState)
{
	if (CurrentState != NewState) {
		OnExitState(CurrentState);
		PreviousState = CurrentState;
		CurrentState = NewState;
		//TODO: ������ Hit�϶� �Ͻ� ������ ���ؼ� ������ �ٽ� �����غ���
		bUseElapsedTime = (NewState == EPlayerState::Hit); 
		StateElapsedTime = 0.f;
		OnEnterState(CurrentState);
	}
}


bool UPlayerStateMachineComponent::IsInAnyState(const TArray<EPlayerState>& StatesToCheck) const
{
	return StatesToCheck.Contains(CurrentState);
}

void UPlayerStateMachineComponent::OnEnterState(EPlayerState State)
{
	
}

void UPlayerStateMachineComponent::OnExitState(EPlayerState State)
{

}
