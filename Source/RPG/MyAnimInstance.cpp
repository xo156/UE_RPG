// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OnwerPawn = TryGetPawnOwner();
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OnwerPawn) { return; }

	if (!MovementComponent) {
		MovementComponent = OnwerPawn->FindComponentByClass<UCharacterMovementComponent>();
	}

	if (!MovementComponent) { return; }

	MoveSpeed = MovementComponent->Velocity.Size();
	bIsInAir = MovementComponent->IsFalling();
}
