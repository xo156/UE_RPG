// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryComponent.h"
#include "CharacterData.h"
#include "MyCharacter.generated.h"

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

	//특정 상황 재생 몽타주
	void PlayAirboneMontage();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//액션
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
	void UpdateLockOnCameraRotation();
	void UpdateLockOnCameraPosition();
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

	//무기
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(TSubclassOf<class UWeaponBaseComponent> WeaponBaseComponentClass);
	class UWeaponBaseComponent* GetCurrentWeapon() const;

	//위젯
	void SetupWidget();

	//데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	int32 PlayerCharacterType;
	float MaxHP;
	float CurrentHP;
	float MaxStamina;
	float CurrentStamina;
	float Damage;
	float UseStamina(float StaminaCost);
	float UseHP(float HPCost);
	void ConsumeStaminaForAction(float StaminaCost);
	bool bHasEnoughStamina(float StaminaCost) const;
	void ConsumeHPForAction(float HPCost);
	bool bHasEnoughHP(float HPCost) const;
	void ChangeMoveSpeed(float DeltaTime);
	void CheckStaminaRecovery(float DeltaTime);
	void RecoveryStaminia(float DeltaTime);

	//AI
	void SetupStimulusSource();

	//캐릭터 상태들
	bool bIsAttack;
	bool bIsMove;
	bool bIsRun;
	bool bIsGuard;
	bool bIsRoll;
	bool bIsLockon;
	bool bIsIdle;
	bool bIsNoDamage;
	bool bIsTalk;

	//소모되는 스테미나
	float RunStaminaCost = 0.2f;
	float JumpStaminaCost = 5.f;
	float AttackStaminaCost = 0.0f;
	float GuardStaminaCost = 10.f;
	float RollStaminaCost = 5.f;

	//getter
	class AItemBase* GetQuickSlotItem();
	class UWeaponBaseComponent* GetCurrentWeaponComponent();
	class UBoxComponent* GetGuardComponent();
	class UUserWidget* GetLockonWidgetInstance();
	class UInventoryQuickSlotWidget* GetInventoryQuickSlotWidgetInstance();

	//setter
	void SetPlayerInfo();
	void SetQuickSlotItem(class AItemBase* NewQuickSlotItem);
	void SetQuickSlotItemAmount(int32 NewAmount);
	void SetQuickSlotItemID(int32 NewID);
	void SetCurrentTalkNPC(class ADialogueNPC* TalkNPC);

	//델리게이트
	FOnPlayerUIUpdated OnPlayerUIUpdated;

	//무기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class UWeaponBaseComponent> WeaponComponent;
		
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

private:
	//카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//무기
	class UWeaponBaseComponent* CurrentWeaponComponent;

	//핸들러
	FTimerHandle ComboCheckTimerHandle;

	//AI
	class UAIPerceptionStimuliSourceComponent* StimulusSource; //Monster가 탐지할 수 있도록

	//이동
	float TargetSpeed;
	float TimeWithoutAction = 0.f; //스테미나 회복 시작까지 걸리는 시간 체크용도
	FVector PreviousLocation;
	FVector CurrentLocation;
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

	//대화
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	float TalkRange = 300.f;
	class ADialogueNPC* CurrentTalkNPC;

	class UDataTable* CharacterDataTable;
};
