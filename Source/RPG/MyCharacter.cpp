// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "PlayerStateMachineComponent.h"
#include "HPActorComponent.h"
#include "StaminaActorComponent.h"
#include "MyPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "DataTableGameInstance.h"
#include "ItemFactory.h"
#include "NonEquipableItem.h"
#include "EquipableItem.h"
#include "EquipSlotType.h"
#include "Weapon.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Kismet/KismetMathLibrary.h"
#include "MonsterBase.h"
#include "DropItem.h"
#include "PlayerHUD.h"
#include "LockonWidget.h"
#include "EquipComponent.h"

// Sets default values
AMyCharacter::AMyCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //이동하는 방향으로 캐릭터를 회전
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bDoCollisionTest = true;
	CameraBoom->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("VGCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	//인벤토리
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	//아이템 줍기
	RootItemBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootItemBox"));
	RootItemBoxComponent->SetupAttachment(RootComponent);
	RootItemBoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootItemBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnRootItemBoxOverlapBegin);
	RootItemBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnRootItemBoxOverlapEnd);

	//자원
	HPActorComponent = CreateDefaultSubobject<UHPActorComponent>(TEXT("Resource"));
	StaminaActorComponent = CreateDefaultSubobject<UStaminaActorComponent>(TEXT("StaminaActorComponent"));
	WalkStaminaCost = 0.f;
	RunStaminaCost = 0.05f;
	JumpStaminaCost = 5.f;
	AttackStaminaCost = 10.f;
	GuardStaminaCost = 5.f;
	RollStaminaCost = 5.f;
	
	//상태 머신
	PlayerStateMachineComponent = CreateDefaultSubobject<UPlayerStateMachineComponent>(TEXT("StateMachine"));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay() {
	Super::BeginPlay();

	SetPlayerInfo();

	//기본 장비 장착
	if (DefaultRightHandWeaponClass) {
		AWeapon* RightWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultRightHandWeaponClass);
		if (RightWeapon) {
			//RightWeapon->InitializeWithData(DefaultRightWeaponData);
			EquipItem(RightWeapon, EEquipSlotType::RightHand);
		}
	}
	if (DefaultLeftHandWeaponClass) {
		AWeapon* LeftWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultLeftHandWeaponClass);
		if (LeftWeapon) {
			//LeftWeapon->InitializeWithData(DefaultLeftWeaponData);
			EquipItem(LeftWeapon, EEquipSlotType::LeftHand);
		}
	}

	//퀵 슬롯
	//QuickSlotList.SetNum(8);//등록 가능 아이템은 총 8개
	//for (int32 Index = 0; Index < QuickSlotList.Num(); Index++) {
	//	QuickSlotList[Index].SlotIndex = Index;
	//}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//스테미나
	CheckStaminaRecovery(DeltaTime);
	ChangeMoveSpeed(DeltaTime);

	if (!PlayerStateMachineComponent)
		return;

	if (bIsRoll && GetVelocity().SizeSquared() < KINDA_SMALL_NUMBER) {
		//구르기가 해제될 때 Idle로
		PlayerStateMachineComponent->ChangeState(EPlayerState::Idle);
	}

	//이전 위치와 현재 위치가 다른 경우 움직임이 있음
	float SpeedSq = GetVelocity().SizeSquared();
	if (SpeedSq > KINDA_SMALL_NUMBER) {
		if (!bIsRun) {
			PlayerStateMachineComponent->ChangeState(EPlayerState::Move);
		}
	}

	//락 온 중이면
	if (bIsLockon && CurrentTarget) {
		UpdateTargetVisibility();
		//대상이 유효하지 않거나 멀어지면
		if (!IsTargetValid(CurrentTarget) || GetDistanceTo(CurrentTarget) > TargetRange) {
			ChangeTarget(nullptr);
			return;
		}
		if (!bIsRoll) { 
			//락 온 중에 구르기 하는중이 아닐때만 대상을 따라 회전
			bUseControllerRotationYaw = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
			//이동 중이거나 가드 중일 때 카메라가 대상을 향하도록 회전
			if (SpeedSq > KINDA_SMALL_NUMBER || bIsGuard) {
				UpdateLockOnCameraRotation();
				UpdateLockOnCameraPosition();
			}
		}
	}
	else {
		//락 온 해제
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (EventInstigator == nullptr)
		return -1.f;
	if (DamageCauser == nullptr)
		return -1.f;

	if (bIsRoll)
		return 0.f;

	if (GetHPActorComponent()->CanConsumeHP(DamageAmount)) {
		//아직 살아있음
		GetHPActorComponent()->ConsumeHP(DamageAmount);
		if (bIsChargingHeavyAttack) {
			//TODO: 차지 캔슬
		}
	}
	else {
		//으악 죽음
		GetHPActorComponent()->ConsumeHP(DamageAmount);
		PlayerDie();
	}

	return DamageAmount;
}

void AMyCharacter::PlayerDie()
{
	if (auto* AnimInstance = GetMesh()->GetAnimInstance()) {
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (auto* PlayerController = Cast<AMyPlayerController>(GetController())) {
			DisableInput(PlayerController);
		}
		if (DieMontage) {
			AnimInstance->Montage_Play(DieMontage);
		}
	}
}

void AMyCharacter::Move(FVector2D InputValue)
{
	if (!PlayerStateMachineComponent)
		return;

	//공격중에 호출 금지
	if (PlayerStateMachineComponent->IsInAnyState({EPlayerState::LightAttack, EPlayerState::HeavyAttack}))
		return;

	PlayerStateMachineComponent->ChangeState(EPlayerState::Move);

	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	LastInputDirection = (ForwardDirection * InputValue.Y + RightDirection * InputValue.X).GetSafeNormal();

	AddMovementInput(ForwardDirection, InputValue.Y);
	AddMovementInput(RightDirection, InputValue.X);
}

void AMyCharacter::ChangeMoveSpeed(float DeltaTime)
{
	//현재 속도를 보간
	float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	float NewSpeed = FMath::Lerp(CurrentSpeed, TargetSpeed, 2.f * DeltaTime);
	if (NewSpeed < WalkSpeed)
		NewSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

	if (bIsRun) {
		if (GetStaminaActorComponent()->CanConsumeStamina(RunStaminaCost)) {
			//달리기 가능하니까 스테미나 소모하도록
			GetStaminaActorComponent()->ConsumeStamina(RunStaminaCost);
		}
		else {
			//달리기 불가능하니까 스테이트 변경
			bIsRun = false;
			TargetSpeed = WalkSpeed;
		}
	}
}

void AMyCharacter::RunStart()
{
	if (!PlayerStateMachineComponent)
		return;

	if (PlayerStateMachineComponent->IsInAnyState({EPlayerState::Move})) {
		bIsRun = true;
		TargetSpeed = RunSpeed;
		ReportNoiseToAI(RunLoudness);
	}

	UE_LOG(LogTemp, Log, TEXT("RunStart"));
}

void AMyCharacter::RunEnd()
{
	if (!PlayerStateMachineComponent)
		return;
	
	if (bIsRun) {
		bIsRun = false;
		TargetSpeed = WalkSpeed;
	}

	UE_LOG(LogTemp, Log, TEXT("RunEnd"));
}

void AMyCharacter::Jump()
{
	if (!PlayerStateMachineComponent || !StaminaActorComponent)
		return;

	if (PlayerStateMachineComponent->IsInAnyState({EPlayerState::Guard,EPlayerState::Idle,EPlayerState::Move})) {
		StaminaActorComponent->ConsumeStamina(JumpStaminaCost);
		ACharacter::Jump();
	}
}

void AMyCharacter::Look(FVector2D InputValue)
{
	if (!InventoryComponent->IsOpen()) {
		AddControllerPitchInput(InputValue.Y);
		AddControllerYawInput(InputValue.X);
	}
}

void AMyCharacter::LightAttackStart()
{
	if (!PlayerStateMachineComponent)
		return;

	UE_LOG(LogTemp, Log, TEXT("AMyCharacter::LightAttackStart()"));

	if (PlayerStateMachineComponent->IsInAnyState({EPlayerState::LightAttack})) {
		if (bIsEnableCombo)
			SetAttackMontageSection();
		return;
	}
	if (auto* PlayerController = Cast<APlayerController>(GetController())) {
		PlayerController->SetIgnoreMoveInput(true);
	}
	PlayerStateMachineComponent->ChangeState(EPlayerState::LightAttack);

	AttackExecute();
}

void AMyCharacter::HeavyAttackChargeStart()
{
	if (!PlayerStateMachineComponent || bIsChargingHeavyAttack)
		return;

	UE_LOG(LogTemp, Log, TEXT("AMyCharacter::HeavyAttackChargeStart()"));

	if (PlayerStateMachineComponent->IsInAnyState({EPlayerState::HeavyAttack})) {
		if (bIsEnableCombo)
			SetAttackMontageSection();
		return;
	}
	if (auto* PlayerController = Cast<APlayerController>(GetController())) {
		PlayerController->SetIgnoreMoveInput(true);
	}
	bIsChargingHeavyAttack = true;
	PlayerStateMachineComponent->ChangeState(EPlayerState::HeavyAttack);

	AttackExecute();
}

void AMyCharacter::SetAttackMontageSection()
{
	if (!GetEquipedRightHandItem() || !GetMesh() || !GetMesh()->GetAnimInstance())
		return;

	auto* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
		return;

	UE_LOG(LogTemp, Log, TEXT("void AMyCharacter::SetAttackMontageSection"));
	if (auto* CurrentActiveMontage = AnimInstance->GetCurrentActiveMontage()) {
		auto CurrentSection = AnimInstance->Montage_GetCurrentSection(AnimInstance->GetCurrentActiveMontage());
		if (!CurrentSection.IsNone()) {
			AnimInstance->Montage_SetNextSection(CurrentSection, NextSectionName, CurrentActiveMontage);
			UE_LOG(LogTemp, Log, TEXT("Montage Next Section: %s"), *NextSectionName.ToString());
		}
	}
}

void AMyCharacter::AttackExecute()
{
	if (!GetEquipedRightHandItem() || !GetMesh() || !HPActorComponent)
		return;

	UE_LOG(LogTemp, Log, TEXT("void AMyCharacter::AttackExecute"));

	auto* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
		return;

	if (bIsChargingHeavyAttack) {
		if (AnimInstance->Montage_IsPlaying(GetEquipedRightHandItem()->GetHeavyAttackMontage()))
			return;
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &AMyCharacter::OnAttackEnded);
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackEnded);
		AnimInstance->Montage_Play(GetEquipedRightHandItem()->GetHeavyAttackMontage());
	}
	else {
		if (AnimInstance->Montage_IsPlaying(GetEquipedRightHandItem()->GetLightAttackMontage()))
			return;
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &AMyCharacter::OnAttackEnded);
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackEnded);
		AnimInstance->Montage_Play(GetEquipedRightHandItem()->GetLightAttackMontage());
	}
	ReportNoiseToAI(AttackLoudness);
}

void AMyCharacter::OnAttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::OnAttackEnded - Montage: %s, Interrupted: %s"),
		   *Montage->GetName(), bInterrupted ? TEXT("True") : TEXT("False"));

	AttackEnd();
}

void AMyCharacter::AttackEnd()
{
	if (!PlayerStateMachineComponent)
		return;

	UE_LOG(LogTemp, Log, TEXT("void AMyCharacter::AttackEnd"));

	PlayerStateMachineComponent->ChangeState(EPlayerState::Idle);
	if (auto* PlayerController = Cast<APlayerController>(GetController())) {
		PlayerController->SetIgnoreMoveInput(false);
	}
	bIsEnableCombo = false;
	bIsChargingHeavyAttack = false;
}

void AMyCharacter::Roll()
{
	if (!StaminaActorComponent)
		return;

	auto* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
		return;

	bIsRoll = true;
	if (!LastInputDirection.IsZero()) {
		SetActorRotation(LastInputDirection.Rotation());
	}
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	AnimInstance->OnMontageEnded.RemoveDynamic(this, &AMyCharacter::OnRollEnded);
	AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnRollEnded);
	AnimInstance->Montage_Play(RollMontage);
	StaminaActorComponent->ConsumeStamina(RollStaminaCost);

	UE_LOG(LogTemp, Log, TEXT("Roll"));
}

void AMyCharacter::OnRollEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!GetMesh() || !GetMesh()->GetAnimInstance())
		return;
	if (Montage == RollMontage) {
		UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::OnRollEnded - Montage: %s, Interrupted: %s"),
			   *Montage->GetName(), bInterrupted ? TEXT("True") : TEXT("False"));

		bIsRoll = false;
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void AMyCharacter::LockOnTarget()
{
	AActor* NewTarget = FindLockOnTarget();

	//기존 타겟과 다르면 변경
	if (bIsLockon && NewTarget) {
		ChangeTarget(NewTarget);
	}
	else {
		ChangeTarget(NewTarget);
	}
}

AActor* AMyCharacter::FindLockOnTarget()
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); //자기 자신 무시
	QueryParams.bTraceComplex = false;
	QueryParams.bReturnPhysicalMaterial = false;

	TArray<FHitResult> HitResults;

	// 트레이스 수행
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation(), 
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(TargetRange),
		QueryParams
	);

	AActor* NearestEnemy = nullptr;
	float NearestDistance = TargetRange;

	if (bHit) {
		for (FHitResult& Hit : HitResults) {
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor->ActorHasTag(FName("Enemy"))) {
				if (HitActor != CurrentTarget) {
					float Distance = GetDistanceTo(HitActor);
					if (Distance < NearestDistance) {
						NearestDistance = Distance;
						NearestEnemy = HitActor;
					}
				}
			}
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can Not Hit Target"));
	}
	return NearestEnemy;
}

void AMyCharacter::UpdateTargetVisibility()
{
	if (!CurrentTarget && !IsTargetValid(CurrentTarget)) {
		ChangeTarget(nullptr);
	}
}

bool AMyCharacter::IsTargetValid(AActor* CheckTarget)
{
	if (!IsValid(CheckTarget) || GetDistanceTo(CheckTarget) > TargetRange) {
		return false;
	}

	if (auto* TargetMonster = Cast<AMonsterBase>(CheckTarget)) {
		//TODO: 타겟이 죽으면 락온 해제
	}

	return true;
}

void AMyCharacter::ChangeTarget(AActor* NewTarget)
{
	if (NewTarget == CurrentTarget) 
		return;

	CurrentTarget = NewTarget;
	bIsLockon = (CurrentTarget != nullptr);

	// HUD에서 락온 위젯을 찾아서 타겟을 설정
	auto* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;

	auto* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
	if (PlayerHUD && PlayerHUD->GetLockonWidget()) {
		PlayerHUD->GetLockonWidget()->SetTarget(CurrentTarget);
	}
}

void AMyCharacter::UpdateLockOnCameraRotation()
{
	if (!CurrentTarget) 
		return;

	FVector DirectionToTarget = (CurrentTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FRotator TargetRotation = DirectionToTarget.Rotation();

	FRotator CurrentRotation = GetControlRotation();
	float AngleDiff = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw);

	if (FMath::Abs(AngleDiff) > MaxTargetAngle) {
		TargetRotation.Yaw = CurrentRotation.Yaw + FMath::Sign(AngleDiff) * MaxTargetAngle;
	}

	if (auto* PlayerController = Cast<AMyPlayerController>(GetController())) {
		PlayerController->SetControlRotation(TargetRotation);
	}
}

void AMyCharacter::UpdateLockOnCameraPosition()
{
	if (auto* PlayerController = Cast<AMyPlayerController>(GetController())) {
		FRotator CurrentRotation = PlayerController->GetControlRotation();
		CurrentRotation.Yaw += (bIsLockon ? 5.0f : -5.0f);
		PlayerController->SetControlRotation(CurrentRotation);
	}
}

void AMyCharacter::Interact()
{
	if (!InventoryComponent || OverlapItems.Num() == 0)
		return;

	TArray<ADropItem*> ItemsToRemove; //다 먹은거
	for (auto* Item : OverlapItems) {
		if (Item) {
			//ADropItem이 여러 개의 아이템을 포함하고 있을 수 있음
			TArray<FDropItemData> DropDatas = Item->GetDropItemDatas();
			for (FDropItemData& DropData : DropDatas) {
				//아이템 종류별로 하나씩 넘겨주기
				InventoryComponent->TryAddItem(DropData);
			}
			ItemsToRemove.Add(Item);
		}
	}
	//인벤토리에 다 옮겼으면 치우기
	for (int32 i = ItemsToRemove.Num() - 1; i >= 0; --i) {
		if (ItemsToRemove[i]) {
			ItemsToRemove[i]->Destroy();
			ItemsToRemove.RemoveAt(i);
		}
	}
}

UInventoryComponent* AMyCharacter::GetInventoryComponent()
{
	return InventoryComponent ? InventoryComponent : nullptr;
}

void AMyCharacter::OnRootItemBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ADropItem::StaticClass())) {
		if (auto* Item = Cast<ADropItem>(OtherActor)) {
			if (!OverlapItems.Contains(Item)) {
				OverlapItems.Add(Item);
			}
		}
	}
}

void AMyCharacter::OnRootItemBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(ADropItem::StaticClass())) {
		if (auto* Item = Cast<ADropItem>(OtherActor)) {
			if (OverlapItems.Contains(Item)) {
				OverlapItems.Remove(Item);
			}
		}
	}
}

void AMyCharacter::EquipItem(AEquipableItem* EquipableItem, EEquipSlotType Slot)
{
	/*if (!EquipableItem)
		return;

	FName SocketName;
	switch (Slot)
	{
	case EEquipSlot::RightHand:
		SocketName = TEXT("Socket_R");
		break;
	case EEquipSlot::LeftHand:
		SocketName = TEXT("Socket_L");
		break;
	default:
		break;
	}

	if (AEquipableItem* Existing = EquippedItems.FindRef(Slot)) {
		UnQuipItem(Slot);
	}

	EquipableItem->SetOwnerCharacter(this);
	EquipableItem->EquipToCharacter(GetMesh(), SocketName);
	if (auto* Weapon = Cast<AWeapon>(EquipableItem)) {
		const FTransform WeaponOffset = Weapon->GetHandOffsetTransform(Slot);
		Weapon->SetActorRelativeLocation(WeaponOffset.GetLocation());
		Weapon->SetActorRelativeRotation(WeaponOffset.GetRotation().Rotator());
	}
	EquippedItems.Add(Slot, EquipableItem);*/
}

void AMyCharacter::UnQuipItem(EEquipSlotType Slot)
{
	/*if (AEquipableItem* Existing = EquippedItems.FindRef(Slot)) {
		Existing->UnEquip();
		Existing->Destroy();
		EquippedItems.Remove(Slot);
	}*/
}

UEquipComponent* AMyCharacter::GetEquipComponent()
{
	return EquipComponent ? EquipComponent : nullptr;
}

void AMyCharacter::CheckStaminaRecovery(float DeltaTime)
{
	if (!PlayerStateMachineComponent || !StaminaActorComponent)
		return;

	if (!bIsRoll && !bIsGuard && CanJump() && (PlayerStateMachineComponent->IsInAnyState({EPlayerState::Guard, EPlayerState::Idle, EPlayerState::Move}))) {
		if (StaminaActorComponent->GetCurrentStamina() < StaminaActorComponent->GetMaxStamina()) {
			StaminaActorComponent->ConsumeStamina(-StaminaRecoveryRate * DeltaTime);
		}
	}
}

void AMyCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource) {
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Hearing>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

void AMyCharacter::ReportNoiseToAI(float Loudness)
{
	static FVector LastNoiseLocation = FVector::ZeroVector;
	if (!LastNoiseLocation.Equals(GetActorLocation(), 10.0f)) {
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), Loudness, this);
		LastNoiseLocation = GetActorLocation();
	}
}

AWeapon* AMyCharacter::GetEquipedRightHandItem() const
{
	//return Cast<AWeapon>(EquippedItems.FindRef(EEquipSlotType::RightHand));
	return nullptr;
}

AWeapon* AMyCharacter::GEtEquipedLeftHandItem() const
{
	//return Cast<AWeapon>(EquippedItems.FindRef(EEquipSlotType::LeftHand));
	return nullptr;
}

UUserWidget* AMyCharacter::GetLockonWidgetInstance()
{
	return LockonWidgetInstance ? LockonWidgetInstance : nullptr;
}

UHPActorComponent* AMyCharacter::GetHPActorComponent()
{
	return HPActorComponent ? HPActorComponent : nullptr;
}

UStaminaActorComponent* AMyCharacter::GetStaminaActorComponent()
{
	return StaminaActorComponent ? StaminaActorComponent : nullptr;
}

UPlayerStateMachineComponent* AMyCharacter::GetPlayerStateMachineComponent()
{
	return PlayerStateMachineComponent ? PlayerStateMachineComponent : nullptr;
}

void AMyCharacter::SetPlayerInfo()
{
	if (!HPActorComponent || !StaminaActorComponent)
		return;

	HPActorComponent->InitHP(100.f);
	StaminaActorComponent->InitStamina(100.f);
}

void AMyCharacter::SetNextSectionName(FName ChangeSectionName)
{
	NextSectionName = ChangeSectionName;
}
