// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	//Locomotion 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion")
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion")
	bool bIsInAir = false;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	class APawn* OnwerPawn;
	class UCharacterMovementComponent* MovementComponent;
};
