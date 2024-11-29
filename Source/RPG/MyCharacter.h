// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryComponent.h"
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
	float Damage;

	float UseStamina(float StaminaCost) {
		if (StaminaCost >= 0) {
			CurrentStamina = FMath::Max(CurrentStamina - StaminaCost, 0.0f);
		}
		else {
			CurrentStamina = FMath::Min(CurrentStamina + StaminaCost, MaxStamina);
		}
		UE_LOG(LogTemp, Warning, TEXT("CurrentStamina: %f"), CurrentStamina);
		return CurrentStamina;
	}
	float UseHP(float HPCost) {
		if (HPCost >= 0) {
			CurrentHP = FMath::Max(CurrentHP - HPCost, 0.0f);
		}
		else {
			CurrentHP = FMath::Min(CurrentHP - HPCost, MaxHP);
		}
		UE_LOG(LogTemp, Warning, TEXT("CurrentHP: %f"), CurrentHP);
		return CurrentHP;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerUIUpdated, float, NewHP, float, NewStamina);
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
	void SetComboAttackTimer();
	void StopComboAttackTimer();
	void AttackEnd();
	void GuardUp();
	void GuardDown();
	void Roll();
	void LockOnTarget();
	AActor* FindLockOnTarget();
	void UpdateTargetVisibility();
	bool IsTargetValid(AActor* CheckTarget);
	void ChangeTarget(AActor* NewTarget);
	void CreateLockonEffect();
	void UpdateLockonEffect();
	void RootItem();
	void OpenInventory();
	UInventoryComponent* GetInventory();
	UFUNCTION()
	void OnRootItemBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnRootItemBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	void QuickSlot();
	void TalkNPC();
	void ShowControlKeysWidget();
	void Close();

	//����
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(TSubclassOf<class UWeaponBaseComponent> WeaponBaseComponentClass);
	class UWeaponBaseComponent* GetCurrentWeapon() const;

	//����
	void SetupWidget();

	//����ü ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	FCharacterStatus CharacterStatus;
	void ConsumeStaminaForAction(float StaminaCost);
	bool bHasEnoughStamina(float StaminaCost) const;
	void ConsumeHPForAction(float HPCost);
	bool bHasEnoughHP(float HPCost) const;
	void ChangeMoveSpeed(float DeltaTime);
	void CheckStaminaRecovery(float DeltaTime);
	void RecoveryStaminia(float DeltaTime);

	//AI
	void SetupStimulusSource();

	//ĳ���� ���µ�
	bool bIsAttack;
	bool bIsMove;
	bool bIsRun;
	bool bIsGuard;
	bool bIsRoll;
	bool bIsLockon;
	bool bIsIdle;
	bool bIsNoDamage;
	bool bIsTalk;

	//�Ҹ�Ǵ� ���׹̳�
	float RunStaminaCost = 0.2f;
	float JumpStaminaCost = 5.f;
	float AttackStaminaCost = 0.0f;
	float GuardStaminaCost = 10.f;
	float RollStaminaCost = 5.f;

	//getter
	AActor* GetCurrentTarget();
	AActor* GetPrevLockOnTarget();
	float GetTargetHeightOffset();
	class AItemBase* GetQuickSlotItem();
	class UWeaponBaseComponent* GetCurrentWeaponComponent();
	class UBoxComponent* GetGuardComponent();

	//setter
	void SetQuickSlotItem(class AItemBase* NewQuickSlotItem);
	void SetQuickSlotItemAmount(int32 NewAmount);
	void SetQuickSlotItemID(int32 NewID);

	//��������Ʈ
	FOnPlayerUIUpdated OnPlayerUIUpdated;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class UWeaponBaseComponent> WeaponComponent;
		
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UInventoryQuickSlotWidget> InventoryQuickSlotWidgetClass;
	class UInventoryQuickSlotWidget* InventoryQuickSlotWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UShowControlKeysWidget> ShowControlKeysWidgetClass;
	class UShowControlKeysWidget* ShowControlKeysWidgetInstance;

	//��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AirboneMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* GuardMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* RollMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* HitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* DieMontage;

private:
	//ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//����
	class UWeaponBaseComponent* CurrentWeaponComponent;

	//�ڵ鷯
	FTimerHandle ComboCheckTimerHandle;

	//AI
	class UAIPerceptionStimuliSourceComponent* StimulusSource; //Monster�� Ž���� �� �ֵ���

	//�̵�
	float TargetSpeed;
	float TimeWithoutAction = 0.f; //���׹̳� ȸ�� ���۱��� �ɸ��� �ð� üũ�뵵
	FVector PreviousLocation;
	FVector CurrentLocation;
	FVector LastInputDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 900.f;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	float TargetHeightOffset = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	float TargetRange = 5000.f;
	AActor* CurrentTarget = nullptr;
	AActor* PrevLockOnTarget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> LockonWidgetClass;
	class UUserWidget* LockonWidgetInstance;

	//������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* RootItemBoxComponent;
	TArray<class ADropItem*> OverlapItems;
	UDataTable* ItemTable;
	class AItemBase* QuickSlotItem;
	int32 QuickSlotItemAmount = 0;
	int32 QuickSlotItemID;

	//��鸲
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* GuardComponent;

	class ANPC* CurrentTalkNPC;
};
