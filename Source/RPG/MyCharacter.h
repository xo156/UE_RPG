// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryComponent.h"
#include "CharacterData.h"
#include "MyCharacter.generated.h"

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

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//액션
	void Move(FVector2D InputValue);
	void RunStart();
	void RunEnd();
	void Jump();
	void Look(FVector2D InputValue);
	void PlayAirboneMontage();
	void AttackStart();
	void SetAttackMontageSection();
	void AttackExecute();
	UFUNCTION()
	void OnAttackEnded(class UAnimMontage* Montage, bool bInterrupted);
	void AttackEnd();
	void GuardUp();
	void GuardDown();
	void Roll();
	UFUNCTION()
	void OnRollEnded(class UAnimMontage* Montage, bool bInterrupted);

	//락 온
	void LockOnTarget();
	AActor* FindLockOnTarget();
	void UpdateTargetVisibility();
	bool IsTargetValid(AActor* CheckTarget);
	void ChangeTarget(AActor* NewTarget);
	void CreateLockonEffect();
	void UpdateLockonEffect();
	void UpdateLockOnCameraRotation();
	void UpdateLockOnCameraPosition();

	//인벤토리
	void RootItem();
	void OpenInventory();
	UInventoryComponent* GetInventory();
	UFUNCTION()
	void OnRootItemBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnRootItemBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	void QuickSlot();

	void Interact();
	void TalkNPC();
	void Close();

	//무기
	UFUNCTION()
	void EquipWeapon(TSubclassOf<class UWeaponBaseComponent> WeaponBaseComponentClass);

	//위젯
	void SetupWidget();

	//Resource
	void ChangeMoveSpeed(float DeltaTime);
	void CheckStaminaRecovery(float DeltaTime);

	void PlayerDie();

	//AI
	void SetupStimulusSource();

	//캐릭터 특수 상태
	bool bIsEnableCombo = false;
	bool bIsRoll = false;
	bool bIsGuard = false;
	bool bIsLockon = false;
	bool bIsNoDamage = false;
	bool bIsTalk = false;

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

	//getter
	class AItemBase* GetQuickSlotItem();
	class UWeaponBaseComponent* GetCurrentWeaponComponent();
	class UBoxComponent* GetGuardComponent();
	class UUserWidget* GetLockonWidgetInstance();
	class UInventoryQuickSlotWidget* GetInventoryQuickSlotWidgetInstance();
	class UResourceComponent* GetResourceComponent();
	class UStateMachineComponent* GetStateMachineComponent();

	//setter
	void SetPlayerInfo();
	void SetNextSectionName(FName ChangeSectionName);
	void SetQuickSlotItem(class AItemBase* NewQuickSlotItem);
	void SetQuickSlotItemAmount(int32 NewAmount);
	void SetQuickSlotItemID(int32 NewID);
	void SetCurrentTalkNPC(class ADialogueNPC* TalkNPC);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//소리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float RunLoudness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AttackLoudness;

	//위젯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UPlayerWidget> PlayerWidgetClass;
	class UPlayerWidget* PlayerWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UInventoryQuickSlotWidget> InventoryQuickSlotWidgetClass;
	class UInventoryQuickSlotWidget* InventoryQuickSlotWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UShowControlKeysWidget> ShowControlKeysWidgetClass;
	class UShowControlKeysWidget* ShowControlKeysWidgetInstance;

	//몽타주
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* TaimMontage;

private:
	//카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//자원
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	int32 PlayerCharacterID;
	struct FCharacterData* CharacterData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource", meta = (AllowPrivateAccess))
	class UResourceComponent* ResourceComponent;
	float StaminaRecoveryRate = 10.f;

	//상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StateMachine", meta = (AllowPrivateAccess="true"))
	class UStateMachineComponent* StateMachineComponent;

	//공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UWeaponBaseComponent> WeaponComponent;
	class UWeaponBaseComponent* CurrentWeaponComponent;
	FName NextSectionName;

	//AI
	class UAIPerceptionStimuliSourceComponent* StimulusSource; //Monster가 탐지할 수 있도록

	//이동
	float TargetSpeed;
	FVector LastInputDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
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

	//구르기
	//bool bPrevUseControllerRotationYaw; //구르기 하기 이전에 값 저장용

	//아이템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* RootItemBoxComponent;
	TArray<class ADropItem*> OverlapItems;
	class AItemBase* QuickSlotItem;
	int32 QuickSlotItemAmount = 0;
	int32 QuickSlotItemID;

	//흔들림
	TSubclassOf<class UCameraShakeBase> CameraShake;

	//가드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* GuardComponent;

	//상호작용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	float InteractRange = 300.f;
	class ADialogueNPC* CurrentTalkNPC;
};
