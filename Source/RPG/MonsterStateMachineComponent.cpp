// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterStateMachineComponent.h"

UMonsterStateMachineComponent::UMonsterStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentState = EMonsterState::Idle;
}

void UMonsterStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	OnEnterState(CurrentState);
}

void UMonsterStateMachineComponent::OnTickState(float DeltaTime)
{
	//�ǰݽ� �Ͻ� ���� ���� �ð� üũ
	if (CurrentState == EMonsterState::Hit && StateElapsedTime >= 1.0f) {
		ChangeState(EMonsterState::Idle);
	}
}

void UMonsterStateMachineComponent::ChangeState(EMonsterState NewState)
{
	if (CurrentState != NewState) {
		OnExitState(CurrentState);
		PreviousState = CurrentState;
		CurrentState = NewState;
		//TODO: ������ Hit�϶� �Ͻ� ������ ���ؼ� ������ �ٽ� �����غ���
		bUseElapsedTime = (NewState == EMonsterState::Hit);
		StateElapsedTime = 0.f;
		OnEnterState(CurrentState);
	}
}

bool UMonsterStateMachineComponent::IsInAnyState(const TArray<EMonsterState>& StatesToCheck) const
{
	return StatesToCheck.Contains(CurrentState);
}

void UMonsterStateMachineComponent::OnEnterState(EMonsterState State)
{

}

void UMonsterStateMachineComponent::OnExitState(EMonsterState State)
{

}
