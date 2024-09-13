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
	float CurrentMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 CurrentMoney;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 MaxMoney;

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerUIUpdated, float, NewHP, float, NewMP, float, NewStamina);
UCLASS()
class RPG_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Ư�� ��Ȳ ��� ��Ÿ��
	void PlayAirboneMontage();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//�׼�
	void Move(FVector2D InputValue);
	void RunStart();
	void RunEnd();
	void Look(FVector2D InputValue);
	void AttackStart();
	void AttackExecute();
	void AttackEnd();
	void Guard();
	void Dodge();
	void LockOnTarget();
	AActor* FindNewTarget();
	void LockOnCamera(float DeltaTime);
	void UnLockOnTarget();
	void RootItem();
	void OpenInventory();
	UInventoryComponent* GetInventory();
	UFUNCTION()
	void OnRootItemBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnRootItemBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	void Close();

	//����
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(TSubclassOf<class UWeaponBaseComponent> WeaponClass);
	class UWeaponBaseComponent* GetCurrentWeapon() const;

	//����
	void SetupWidget();

	//����ü ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	FCharacterStatus CharacterStatus;
	void ConsumeStaminaForAction(float StaminaCost);
	bool bHasEnoughStamina(float StaminaCost) const;
	void ConsumeMPForAction(float MPCost);
	bool bHasEnoughMP(float MPCost) const;
	void ConsumeHPForAction(float HPCost);
	bool bHasEnoughHP(float HPCost) const;
	void ChangeMoveSpeed(float DeltaTime);
	void CheckStaminaRecovery(float DeltaTime);
	void RecoveryStaminia(float DeltaTime);

	//AI
	void SetupStimulusSource();

	//����

	//ĳ���� ���µ�
	bool bIsAttack;
	bool bIsMove;
	bool bIsRun;
	bool bIsGuard;
	bool bIsDodge;
	bool bIsLockOnTarget;

	//�Ҹ�Ǵ� ���׹̳�
	float RunStaminaCost = 0.2f;
	float JumpStaminaCost = 5.f;
	//float AttackStaminaCost = 6.f;
	float AttackStaminaCost = 0.0f;
	float GuardStaminaCost = 10.f;
	float DodgeStaminaCost = 5.f;

	//�׽�Ʈ
	void TEST();

	//��������Ʈ
	FOnPlayerUIUpdated OnPlayerUIUpdated;

	//����
	AActor* LockedOnTarget = nullptr; 
		
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float RunLoudness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AttackLoudness;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UPlayerWidget> PlayerWidgetClass;
	class UPlayerWidget* PlayerWidgetInstance;

	//��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AirboneMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* BlockMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* DodgeMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* HitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* DieMontage;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class UWeaponBaseComponent> BareHand;

private:
	//ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//����
	class UWeaponBaseComponent* CurrentWeapon;

	//�ڵ鷯
	FTimerHandle ComboCheckTimerHandle;

	//AI
	class UAIPerceptionStimuliSourceComponent* StimulusSource; //Monster�� Ž���� �� �ֵ���

	//�̵��ӵ�
	float TargetSpeed;
	float TimeWithoutAction = 0.f; //���׹̳� ȸ�� ���۱��� �ɸ��� �ð� üũ�뵵

	//����ü �߰� �����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 900.f;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	float LockOnConeAngle = 45.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	float LockOnConeRadius = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	float MaxLockOnDist = 300.f;
	FVector PreviousLocation;
	FVector CurrentLocation;

	//������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* RootItemBoxComponent;
	TArray<class ADropItem*> OverlapItems;
};
