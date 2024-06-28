// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStatus
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxMP;

	float ConsumeStamina(float StaminaCost) {
		if (StaminaCost > 0) {
			CurrentStamina = FMath::Max(CurrentStamina - StaminaCost, 0.0f);
		}
		UE_LOG(LogTemp, Warning, TEXT("CurrentStamina: %f"), CurrentStamina);
		return CurrentStamina;
	}
	float ConsumeMP(float MPCost) {
		if (MPCost > 0) {
			CurrentMP = FMath::Max(CurrentMP - MPCost, 0.0f);
		}
		UE_LOG(LogTemp, Warning, TEXT("CurrentMP: %f"), CurrentMP);
		return CurrentMP;
	}
};
UCLASS()
class RPG_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ConsumeStaminaForAction(float StaminaCost);
	bool bHasEnoughStamina(float StaminaCost) const;
	void ConsumeMPForAction(float MPCost);
	bool bHasEnoughMP(float MPCost) const;

	void ChangeMoveSpeed(float DeltaTime);
	void CheckStaminaRecovery(float DeltaTime);
	void RecoveryStaminia(float DeltaTime);

	void PlayAirboneMontage();

	void Move(FVector2D InputValue);
	void RunStart();
	void RunEnd();
	void Look(FVector2D InputValue);
	void Attack();
	void ResetAttackCount();
	void Block();
	void Dodge();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(TSubclassOf<class UWeaponBaseComponent> WeaponClass);

	void SetupStimulusSource();

	class UWeaponBaseComponent* GetCurrentWeapon() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	FCharacterStatus CharacterStatus;

	//캐릭터 상태들
	bool bIsAttacking;
	bool bIsRunning;
	bool bIsBloacking;
	bool bIsDodging;

	//소모되는 스테미나
	float RunStaminaCost = 0.2f;
	float JumpStaminaCost = 5.f;
	float AttackStaminaCost = 10.f;
	float BlockStaminaCost = 10.f;
	float DodgeStaminaCost = 5.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AirboneMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* BlockMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class UWeaponBaseComponent> BareHand;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* VGCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	class UWeaponBaseComponent* CurrentWeapon;

	FTimerHandle ComboCheckTimerHandle;
	FTimerHandle DodgeHandle;

	class UAIPerceptionStimuliSourceComponent* StimulusSource; //NormalMonster가 탐지할 수 있도록

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 900.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float DodgeSpeed = 2500.f;

	float TargetSpeed;
	float TimeWithoutAction = 0.f; //스테미나 회복 시작까지 걸리는 시간 체크용도
};
