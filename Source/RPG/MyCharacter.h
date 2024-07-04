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

	float UseStamina(float StaminaCost) {
		if (StaminaCost >= 0) {
			CurrentStamina = FMath::Max(CurrentStamina - StaminaCost, 0.0f);
		}
		UE_LOG(LogTemp, Warning, TEXT("CurrentStamina: %f"), CurrentStamina);
		return CurrentStamina;
	}
	float UseMP(float MPCost) {
		if (MPCost >= 0) {
			CurrentMP = FMath::Max(CurrentMP - MPCost, 0.0f);
		}
		UE_LOG(LogTemp, Warning, TEXT("CurrentMP: %f"), CurrentMP);
		return CurrentMP;
	}
	float UseHP(float HPCost) {
		if (HPCost >= 0) {
			CurrentHP = FMath::Max(CurrentHP - HPCost, 0.0f);
		}
		UE_LOG(LogTemp, Warning, TEXT("CurrentHP: %f"), CurrentHP);
		return CurrentHP;
	}
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUIUpdated, float, NewHP, float, NewMP, float, NewStamina);
UCLASS()
class RPG_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//특정 상황 재생 몽타주
	void PlayAirboneMontage();

	//액션
	void Move(FVector2D InputValue);
	void RunStart();
	void RunEnd();
	void Look(FVector2D InputValue);
	void Attack();
	void ResetAttackCount();
	void Guard();
	void Dodge();
	void LockOnTarget();
	void UnLockOnTarget();
	AActor* FindNewTarget() const;

	//무기
	void EquipWeapon(TSubclassOf<class UWeaponBaseComponent> WeaponClass);
	class UWeaponBaseComponent* GetCurrentWeapon() const;
	UFUNCTION(BlueprintCallable, Category = "Weapon")

	//위젯
	void SetupWidget();

	//구조체 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	FCharacterStatus CharacterStatus;
	void ConsumeStaminaForAction(float StaminaCost);
	bool bHasEnoughStamina(float StaminaCost) const;
	void ConsumeMPForAction(float MPCost);
	bool bHasEnoughMP(float MPCost) const;
	void ConsumeHPForAction(float HPCost);
	bool bHasEnoughHP(float HPCost);
	void ChangeMoveSpeed(float DeltaTime);
	void CheckStaminaRecovery(float DeltaTime);
	void RecoveryStaminia(float DeltaTime);

	//AI
	void SetupStimulusSource();

	//캐릭터 상태들
	bool bIsAttack;
	bool bIsRun;
	bool bIsGuard;
	bool bIsDodge;
	bool bIsLockOnTarget;

	//소모되는 스테미나
	float RunStaminaCost = 0.2f;
	float JumpStaminaCost = 5.f;
	float AttackStaminaCost = 6.f;
	float BlockStaminaCost = 10.f;
	float DodgeStaminaCost = 5.f;

	//테스트
	void TESTSTATUS();

	//델리게이트
	FOnUIUpdated OnUIUpdated;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//위젯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UPlayerStatusUserWidget> PlayerStatusWidgetClass;

	class UPlayerStatusUserWidget* PlayerStatusWidgetInstance;

	//몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AirboneMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* BlockMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* DodgeMontage;

	//무기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class UWeaponBaseComponent> BareHand;

	AActor* LockedOnTarget; //현재 락온중인 타겟

private:
	//카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//무기
	class UWeaponBaseComponent* CurrentWeapon;

	//핸들러
	FTimerHandle ComboCheckTimerHandle;
	FTimerHandle DodgeHandle;

	//AI
	class UAIPerceptionStimuliSourceComponent* StimulusSource; //NormalMonster가 탐지할 수 있도록

	//구조체 추가 고민중
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 900.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float DodgeSpeed = 2500.f;

	//이동속도
	float TargetSpeed;
	float TimeWithoutAction = 0.f; //스테미나 회복 시작까지 걸리는 시간 체크용도

	//락온
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	float LockOnConeAngle = 45.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	float LockOnConeRadius = 1000.f;
};
