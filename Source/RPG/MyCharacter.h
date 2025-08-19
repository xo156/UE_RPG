// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QuickslotData.h"
#include "MyCharacter.generated.h"

UCLASS()
class RPG_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void PlayerDie();

	//액션
	void Move(FVector2D InputValue);
	void ChangeMoveSpeed(float DeltaTime);
	void RunStart();
	void RunEnd();
	void Jump();
	void Look(FVector2D InputValue);
	void LightAttackStart();
	void HeavyAttackChargeStart();
	void SetAttackMontageSection();
	void AttackExecute();
	UFUNCTION()
	void OnAttackEnded(class UAnimMontage* Montage, bool bInterrupted);
	void AttackEnd();
	void Roll();
	UFUNCTION()
	void OnRollEnded(class UAnimMontage* Montage, bool bInterrupted);

	//락 온
	void LockOnTarget();
	AActor* FindLockOnTarget();
	void UpdateTargetVisibility();
	bool IsTargetValid(AActor* CheckTarget);
	void ChangeTarget(AActor* NewTarget);
	void UpdateLockOnCameraRotation();
	void UpdateLockOnCameraPosition();

	//상호작용
	void Interact();

	//아이템 줍기
	class UInventoryComponent* GetInventoryComponent();
	UFUNCTION()
	void OnRootItemBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnRootItemBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	
	//퀵 슬롯
	//bool AddToQuickSlot(int32 SlotIndex, int32 ItemTableID, class AItemBase* ItemInstance);
	//bool UseQuickSlot(int32 SlotIndex);

	//장비
	UFUNCTION()
	void EquipItem(class AEquipableItem* EquipableItem, EEquipSlotType Slot);
	void UnQuipItem(EEquipSlotType Slot);
	class UEquipComponent* GetEquipComponent();

	//자원
	void CheckStaminaRecovery(float DeltaTime);

	//AI
	void SetupStimulusSource();
	void ReportNoiseToAI(float Loudness);

	//getter
	class AWeapon* GetEquipedRightHandItem() const;
	class AWeapon* GEtEquipedLeftHandItem() const;
	class UUserWidget* GetLockonWidgetInstance();
	class UHPActorComponent* GetHPActorComponent();
	class UStaminaActorComponent* GetStaminaActorComponent();
	class UPlayerStateMachineComponent* GetPlayerStateMachineComponent();

	//setter
	void SetPlayerInfo();
	void SetNextSectionName(FName ChangeSectionName);

	//캐릭터 특수 상태
	bool bIsEnableCombo = false;
	bool bIsChargingHeavyAttack = false;
	bool bIsRun = false;
	bool bIsRoll = false;
	bool bIsGuard = false;
	bool bIsLockon = false;
	bool bIsNoDamage = false;

	//캐릭터 스테미나 소모량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player/Stamina")
	float WalkStaminaCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player/Stamina")
	float RunStaminaCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player/Stamina")
	float JumpStaminaCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player/Stamina")
	float AttackStaminaCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player/Stamina")
	float GuardStaminaCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player/Stamina")
	float RollStaminaCost;

protected:
	//소리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float RunLoudness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AttackLoudness;

	//기본 장비
	UPROPERTY(EditDefaultsOnly, Category = "Default Equipment")
	TSubclassOf<AWeapon> DefaultRightHandWeaponClass;
	UPROPERTY(EditDefaultsOnly, Category = "Default Equipment")
	TSubclassOf<AWeapon> DefaultLeftHandWeaponClass;

	//몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AirboneMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* RollMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* HitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* DieMontage;

private:
	//카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//자원
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource", meta = (AllowPrivateAccess))
	class UHPActorComponent* HPActorComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource", meta = (AllowPrivateAccess))
	class UStaminaActorComponent* StaminaActorComponent;
	float StaminaRecoveryRate = 10.f;

	//상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StateMachine", meta = (AllowPrivateAccess="true"))
	class UPlayerStateMachineComponent* PlayerStateMachineComponent;

	//공격
	FName NextSectionName;

	//장비
	class AWeapon* CurrentRightWeapon;
	class AWeapon* CurrentLeftWeapon;
	TMap<EEquipSlotType, class AEquipableItem*> EquippedItems;

	//AI
	class UAIPerceptionStimuliSourceComponent* StimulusSource; //Monster가 탐지할 수 있도록

	//이동
	float TargetSpeed;
	FVector LastInputDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSpeed", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSpeed", meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 900.f;

	//락온
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	float TargetRange = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	float MaxTargetAngle = 45.f;
	AActor* CurrentTarget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> LockonWidgetClass;
	class UUserWidget* LockonWidgetInstance;

	//인벤토리 및 아이템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* RootItemBoxComponent;
	TArray<class ADropItem*> OverlapItems;

	//장비
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UEquipComponent* EquipComponent;
	
	//퀵 슬롯
	class AItemBase* QuickSlotItem;
	int32 QuickSlotItemAmount = 0;
	int32 QuickSlotItemID;

	//TArray<FQuickSlotData> QuickSlotList;
};
