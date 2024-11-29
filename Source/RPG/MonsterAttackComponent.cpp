// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Monster.h"

// Sets default values for this component's properties
UMonsterAttackComponent::UMonsterAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UMonsterAttackComponent::MonsterStartAttack()
{
	UE_LOG(LogTemp, Log, TEXT("UMonsterAttackComponent::MonsterStartAttack()"));
	if(OwnerMonster->bIsMonsterAttack)
		OwnerMonster->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UMonsterAttackComponent::MonsterExecuteAttack()
{
	UE_LOG(LogTemp, Log, TEXT("UMonsterAttackComponent::MonsterExecuteAttack()"));
	if (auto* AnimInstance = OwnerMonster->GetMesh()->GetAnimInstance()) {
		if (OwnerMonster->GetMonsterAttackMontage()) {
			AnimInstance->Montage_Play(OwnerMonster->GetMonsterAttackMontage());
			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindUObject(OwnerMonster, &AMonster::OnAttackMontageEnded);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, OwnerMonster->GetMonsterAttackMontage());
		}
	}
}

void UMonsterAttackComponent::MonsterEndAttack()
{
	UE_LOG(LogTemp, Log, TEXT("UMonsterAttackComponent::MonsterEndAttack()"));
	OwnerMonster->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UMonsterAttackComponent::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Log, TEXT("UMonsterAttackComponent::OnAttackMontageEnded"));
	OwnerMonster->MonsterAttackEnd();
}


// Called when the game starts
void UMonsterAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	if(!OwnerMonster)
		OwnerMonster = Cast<AMonster>(GetOwner());
}


// Called every frame
void UMonsterAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

