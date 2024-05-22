// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
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
	void ResetAttackCount();

public:
	bool bIsAttacking;

protected:
	UPROPERTY(EditAnywhere, Category = "Combo")
	TSubclassOf<class AWeaponBase> WeaponClass;

	class AWeaponBase* CurrentWeapon;

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

private:
	FTimerHandle ComboCheckTimerHandle;
	class AMyCharacter* MyCharacter;
};
