// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "WeaponBase.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
class AMyCharacter;

UCLASS()
class RPG_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

	AMyCharacter* GetCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo")
	TSubclassOf<AWeaponBase> Weapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//빙의하는 타이밍에(캐릭터에 컨트롤러가 들어갈 때)
	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void PickUPItem(const FInputActionValue& Value);

	void CountZero();

protected:
	//인풋 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	//몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AttackMontage1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AttackMontage2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AttackMontage3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	int32 CurrentComboCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	float WaitComboTime = 1.f;

private:
	class AMyCharacter* MyCharacter;
};
