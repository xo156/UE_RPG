// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "WeaponBaseComponent.h"
#include "Weapon.h"
#include "MyPlayerController.h"
#include "Components/BoxComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "PlayerWidget.h"
#include "DropItem.h"
#include "InventoryWidget.h"
#include "InventoryItemAction.h"
#include "Kismet/KismetMathLibrary.h"
#include "Monster.h"
#include "DataTableGameInstance.h"
#include "InventoryQuickSlotWidget.h"
#include "DialogueNPC.h"
#include "ResourceComponent.h"
#include "StateMachineComponent.h"

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

	//위젯
	PlayerWidgetClass = UPlayerWidget::StaticClass();

	//인벤토리
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	//아이템 줍기
	RootItemBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootItemBox"));
	RootItemBoxComponent->SetupAttachment(RootComponent);
	RootItemBoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootItemBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnRootItemBoxOverlapBegin);
	RootItemBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnRootItemBoxOverlapEnd);

	//막기
	GuardComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("GuardBox"));
	GuardComponent->SetupAttachment(RootComponent);
	GuardComponent->SetCollisionProfileName(TEXT("NoCollision"));

	//자원
	ResourceComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("Resource"));
	WalkStaminaCost = 0.f;
	RunStaminaCost = 0.05f;
	JumpStaminaCost = 5.f;
	AttackStaminaCost = 10.f;
	GuardStaminaCost = 5.f;
	RollStaminaCost = 5.f;
	
	//상태
	StateMachineComponent = CreateDefaultSubobject<UStateMachineComponent>(TEXT("StateMachine"));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay() {
	Super::BeginPlay();

	if (auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
		CameraShake = GameInstance->GetCameraShake();
		CharacterData = GameInstance->GetCharacterInfo(PlayerCharacterID);
		SetPlayerInfo();
	}

	if (WeaponComponent) {
		EquipWeapon(WeaponComponent);
	}

	SetupWidget();

	if (InventoryComponent) {
		InventoryComponent->CreateInventoryWidget();
	}

	if (StateMachineComponent) {
		StateMachineComponent->SetCurrentState(EPlayerState::PlayerIdle);
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//스테미나
	CheckStaminaRecovery(DeltaTime);
	ChangeMoveSpeed(DeltaTime);

	if (!StateMachineComponent)
		return;

	if (bIsRoll && GetVelocity().SizeSquared() < KINDA_SMALL_NUMBER) {
		//구르기가 해제될 때 Idle로
		StateMachineComponent->SetCurrentState(EPlayerState::PlayerIdle);
	}

	//이전 위치와 현재 위치가 다른 경우 움직임이 있음
	float SpeedSq = GetVelocity().SizeSquared();
	if (SpeedSq > KINDA_SMALL_NUMBER) {
		if (StateMachineComponent->GetCurrentState() != EPlayerState::PlayerRun) {
			StateMachineComponent->SetCurrentState(EPlayerState::PlayerWalk);
		}
	}

	//락 온 중이면
	if (bIsLockon && CurrentTarget) {
		UpdateTargetVisibility();
		//대상이 유효하지 않거나 멀어지면
		if (!IsTargetValid(CurrentTarget) || GetDistanceTo(CurrentTarget) > TargetRange) {
			ChangeTarget(nullptr);
			UpdateLockonEffect(); //락 온 해제
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
		if (LockonWidgetInstance) {
			UpdateLockonEffect();
		}
	}
	else {
		//락 온 해제
		UpdateLockonEffect();
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

	if (auto* PlayerController = Cast<AMyPlayerController>(GetController())) {
		PlayerController->ClientPlayCameraShake(CameraShake);
	}

	if (GetResourceComponent()->bCanConsumeHealth(DamageAmount)) {
		//아직 살아있음
		GetResourceComponent()->ConsumeHP(DamageAmount);
	}
	else {
		//으악 죽음
		GetResourceComponent()->ConsumeHP(DamageAmount);
		PlayerDie();
	}

	return DamageAmount;
}

void AMyCharacter::Move(FVector2D InputValue)
{
	GetStateMachineComponent()->SetCurrentState(EPlayerState::PlayerWalk);

	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	LastInputDirection = (ForwardDirection * InputValue.Y + RightDirection * InputValue.X).GetSafeNormal();

	AddMovementInput(ForwardDirection, InputValue.Y);
	AddMovementInput(RightDirection, InputValue.X);
}

void AMyCharacter::RunStart()
{
	if (GetStateMachineComponent()->GetCurrentState() != EPlayerState::PlayerRun) {
		GetStateMachineComponent()->SetCurrentState(EPlayerState::PlayerRun);
	}

	TargetSpeed = RunSpeed;
	//같은 위치에서 이미 호출되었다면 넘어가도록
	FVector LastNoiseLocation = FVector::ZeroVector;
	if (!LastNoiseLocation.Equals(GetActorLocation(), 10.0f)) {
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), RunLoudness, this);
		LastNoiseLocation = GetActorLocation();
	}
}

void AMyCharacter::RunEnd()
{
	GetStateMachineComponent()->SetCurrentState(EPlayerState::PlayerWalk);

	TargetSpeed = WalkSpeed;
}

void AMyCharacter::Jump()
{
	GetStateMachineComponent()->SetCurrentState(EPlayerState::PlayerJump);

	GetResourceComponent()->ConsumeStamina(JumpStaminaCost);
	ACharacter::Jump();
}

void AMyCharacter::Look(FVector2D InputValue)
{
	if (!InventoryComponent->bIsOpen) {
		AddControllerPitchInput(InputValue.Y);
		AddControllerYawInput(InputValue.X);
	}
}

void AMyCharacter::PlayAirboneMontage()
{
	if (GetCharacterMovement()->IsFalling()) {
		PlayAnimMontage(AirboneMontage);
	}
}

void AMyCharacter::AttackStart()
{
	if (!StateMachineComponent)
		return;

	UE_LOG(LogTemp, Log, TEXT("void AMyCharacter::AttackStart"));

	if (StateMachineComponent->GetCurrentState() == EPlayerState::PlayerAttack) {
		if (bIsEnableCombo)
			SetAttackMontageSection();
		return;
	}
	AttackExecute();
}

void AMyCharacter::SetAttackMontageSection()
{
	if (!CurrentWeaponComponent || !ResourceComponent || !GetMesh() || !GetMesh()->GetAnimInstance())
		return;

	UE_LOG(LogTemp, Log, TEXT("void AMyCharacter::SetAttackMontageSection"));
	if (auto* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (auto* CurrentActiveMontage = AnimInstance->GetCurrentActiveMontage()) {
			UE_LOG(LogTemp, Log, TEXT("CurrentActiveMontage: %s"), *CurrentActiveMontage->GetName());
			auto CurrentSection = AnimInstance->Montage_GetCurrentSection(AnimInstance->GetCurrentActiveMontage());
			if (!CurrentSection.IsNone()) {
				AnimInstance->Montage_SetNextSection(CurrentSection, NextSectionName, CurrentWeaponComponent->LightAttackMontage);
				UE_LOG(LogTemp, Log, TEXT("Montage Next Section: %s"), *NextSectionName.ToString());
			}
		}
	}
}

void AMyCharacter::AttackExecute()
{
	if (!CurrentWeaponComponent || !GetMesh() || !ResourceComponent) 
		return;

	if (StateMachineComponent->GetCurrentState() == EPlayerState::PlayerAttack)
		return;

	UE_LOG(LogTemp, Log, TEXT("void AMyCharacter::AttackExecute"));
	StateMachineComponent->SetCurrentState(EPlayerState::PlayerAttack);

	auto* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
		return;

	//기존 바인딩이 있다면 제거 (이전 공격 애니메이션이 끝나기 전에 공격을 다시 하면 중복 호출 방지)
	if (AnimInstance->OnMontageEnded.IsAlreadyBound(this, &AMyCharacter::OnAttackEnded)) {
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &AMyCharacter::OnAttackEnded);
	}
	AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackEnded);
	AnimInstance->Montage_Play(CurrentWeaponComponent->LightAttackMontage);
	//같은 위치에서 이미 호출되었다면 넘어가도록
	FVector LastNoiseLocation = FVector::ZeroVector;
	if (!LastNoiseLocation.Equals(GetActorLocation(), 10.0f)) {
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), RunLoudness, this);
		LastNoiseLocation = GetActorLocation();
	}
}

void AMyCharacter::OnAttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::OnAttackEnded - Montage: %s, Interrupted: %s"),
		   *Montage->GetName(), bInterrupted ? TEXT("True") : TEXT("False"));

	if(!bIsEnableCombo)
		AttackEnd();
}

void AMyCharacter::AttackEnd()
{
	UE_LOG(LogTemp, Log, TEXT("void AMyCharacter::AttackEnd"));

	GetStateMachineComponent()->SetCurrentState(EPlayerState::PlayerIdle);
	bIsEnableCombo = false;
}

void AMyCharacter::GuardUp()
{
	if (!InventoryComponent->bIsOpen) {
		bIsGuard = true;
		if (auto* AnimInstance = GetMesh()->GetAnimInstance()) {
			GuardComponent->SetCollisionProfileName(TEXT("Pawn"));
			GuardComponent->SetVisibility(true);
			AnimInstance->Montage_Play(GuardMontage);
		}
	}
}

void AMyCharacter::GuardDown()
{
	bIsGuard = false;
	GuardComponent->SetCollisionProfileName(TEXT("NoCollision"));
	GuardComponent->SetVisibility(false);
}

void AMyCharacter::Roll()
{
	auto* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || InventoryComponent->bIsOpen)
		return;

	bIsRoll = true;
	if (!LastInputDirection.IsZero()) {
		SetActorRotation(LastInputDirection.Rotation());
	}
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//기존 바인딩이 있다면 제거 (이전 구르기 애니메이션이 끝나기 전에 구르기를 다시 하면 중복 호출 방지)
	if (AnimInstance->OnMontageEnded.IsAlreadyBound(this, &AMyCharacter::OnRollEnded)) {
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &AMyCharacter::OnRollEnded);
	}
	AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnRollEnded);
	AnimInstance->Montage_Play(RollMontage);
	GetResourceComponent()->ConsumeStamina(RollStaminaCost);
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

	if (AMonster* TargetMonster = Cast<AMonster>(CheckTarget)) {
		if (TargetMonster->bIsMonsterDead) {
			return false;
		}
	}

	return true;
}

void AMyCharacter::ChangeTarget(AActor* NewTarget)
{
	if (NewTarget == CurrentTarget) 
		return;

	CurrentTarget = NewTarget;
	bIsLockon = (CurrentTarget != nullptr);

	if (!bIsLockon) {
		if (LockonWidgetInstance) {
			LockonWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else {
		CreateLockonEffect();
	}
}

void AMyCharacter::CreateLockonEffect()
{
	if (LockonWidgetInstance) {
		UpdateLockonEffect();
	}
	else {
		if (LockonWidgetClass) {
			LockonWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), LockonWidgetClass);
			if (LockonWidgetInstance) {
				LockonWidgetInstance->AddToViewport();
				UpdateLockonEffect();
			}
		}
	}
}

void AMyCharacter::UpdateLockonEffect()
{
	if (LockonWidgetInstance && CurrentTarget) {
		FVector TargetLocation = CurrentTarget->GetActorLocation();
		FVector2D ScreenPosition;
		UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), TargetLocation, ScreenPosition);
		LockonWidgetInstance->SetVisibility(ESlateVisibility::Visible);

		LockonWidgetInstance->SetPositionInViewport(ScreenPosition);
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

	if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController())) {
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

void AMyCharacter::RootItem()
{
	if (InventoryComponent) {
		TArray<ADropItem*> ItemsToRemove;
		if (OverlapItems.Num() > 0) {
			for (ADropItem* Item : OverlapItems) {
				if (Item) {
					bool bAdded = InventoryComponent->TryAddItem(Item);
					ItemsToRemove.Add(Item);
				}
			}
			for (ADropItem* DestroyItem : ItemsToRemove) {
				ItemsToRemove.Remove(DestroyItem);
				DestroyItem->Destroy();
			}
		}
	}
}

void AMyCharacter::OpenInventory()
{
	if (InventoryComponent) {
		if (InventoryComponent->bIsOpen) {
			InventoryComponent->CloseInventoryWidget();
			if (auto* PlayerController = Cast<AMyPlayerController>(GetController())) {
				PlayerController->HideItemAction();
			}
		}
		else {
			InventoryComponent->OpenInventoryWidget();
		}
	}
}

UInventoryComponent* AMyCharacter::GetInventory()
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

void AMyCharacter::QuickSlot()
{
	if (QuickSlotItem && InventoryComponent && QuickSlotItemAmount > 0) {
		QuickSlotItem->Use();
		InventoryComponent->RemoveItem(QuickSlotItemID, 1);
		SetQuickSlotItemAmount(--QuickSlotItemAmount);
		if (InventoryQuickSlotWidgetInstance) {
			if (QuickSlotItemAmount <= 0) {
				InventoryQuickSlotWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
				QuickSlotItemAmount = 0;
				QuickSlotItem = nullptr;
			}
			else {
				InventoryComponent->InventoryWidget->UpdateInventoryWidget(InventoryComponent);
				InventoryQuickSlotWidgetInstance->UpdateQuickSlotItemAmount(QuickSlotItemAmount);
			}
		}
	}
}

void AMyCharacter::Interact()
{
	//상호작용 가능한 NPC 또는 동물 찾기
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); //자기 자신 무시
	QueryParams.bTraceComplex = false;
	QueryParams.bReturnPhysicalMaterial = false;

	TArray<FHitResult> HitResults;

	//트레이스 수행
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(InteractRange),
		QueryParams
	);

	if (bHit) {
		for (FHitResult& Hit : HitResults) {
			AActor* HitActor = Hit.GetActor();
			if (HitActor) {
				if (HitActor->ActorHasTag(FName("NPC"))) {
					if (auto* NPC = Cast<ADialogueNPC>(HitActor)) {
						CurrentTalkNPC = NPC;
						TalkNPC();
						return; //NPC가 발견되면 함수 종료
					}
				}
			}
		}
	}
}

void AMyCharacter::TalkNPC()
{
	if (CurrentTalkNPC) {
		if (bIsTalk) {
			CurrentTalkNPC->GetDialogueComponent()->NextDialogue();
		}
		else {
			CurrentTalkNPC->ShowDialogues();
			bIsTalk = true;
		}
	}
}

void AMyCharacter::Close()
{
	if (InventoryComponent->InventoryWidget) {
		InventoryComponent->InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMyCharacter::EquipWeapon(TSubclassOf<class UWeaponBaseComponent> WeaponBaseComponentClass)
{
	if (WeaponBaseComponentClass) {
		if (CurrentWeaponComponent) {
			if (InventoryComponent) {
				if (auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
					const TMap<int32, FItemData*>& ItemCache = GameInstance->GetItemCache();
					for (const auto& ItemPair : ItemCache) {
						FItemData* ItemData = ItemPair.Value;
						if (ItemData) {
							for (const auto& ItemClass : ItemData->ItemClass) {
								if (ItemClass && CurrentWeaponComponent->GetRightHandWeaponInstance()->IsA(ItemClass)) {
									if (auto NowEquipWeapon = GetWorld()->SpawnActor<ADropItem>(ADropItem::StaticClass())) {
										FDropItemData NowEquipWeaponData;
										NowEquipWeaponData.Amount = 1;
										NowEquipWeaponData.bCounterble = false;
										NowEquipWeaponData.ItemID = ItemData->ItemID;
										NowEquipWeapon->SetDropItem(NowEquipWeaponData);
										InventoryComponent->TryAddItem(NowEquipWeapon);
									}
									break;
								}
							}
						}
					}
				}
			}
			CurrentWeaponComponent->GetRightHandWeaponInstance()->Destroy();
			CurrentWeaponComponent->GetLeftHandWeaponInstance()->Destroy();
			CurrentWeaponComponent = nullptr;
		}
		CurrentWeaponComponent = NewObject<UWeaponBaseComponent>(this, WeaponBaseComponentClass);
		if (CurrentWeaponComponent) {
			CurrentWeaponComponent->SetOwnerCharacter(this);
		}
	}
}

void AMyCharacter::SetupWidget()
{
	if (PlayerWidgetClass) {
		PlayerWidgetInstance = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);
		if (PlayerWidgetInstance) {
			PlayerWidgetInstance->AddToViewport();
			if (ResourceComponent) {
				PlayerWidgetInstance->UpdateHP(ResourceComponent->GetCurrentHealth(), ResourceComponent->GetMaxHealth());
				PlayerWidgetInstance->UpdateStamina(ResourceComponent->GetCurrentStamina(), ResourceComponent->GetMaxStamina());
			}
		}
	}

	if (InventoryQuickSlotWidgetClass) {
		InventoryQuickSlotWidgetInstance = CreateWidget<UInventoryQuickSlotWidget>(GetWorld(), InventoryQuickSlotWidgetClass);
		if (InventoryQuickSlotWidgetInstance) {
			InventoryQuickSlotWidgetInstance->AddToViewport();
			InventoryQuickSlotWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMyCharacter::ChangeMoveSpeed(float DeltaTime)
{
	//현재 속도를 보간
	float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	float NewSpeed = FMath::Lerp(CurrentSpeed, TargetSpeed, 2.f * DeltaTime);
	if (NewSpeed < WalkSpeed)
		NewSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

	if (GetStateMachineComponent()->GetCurrentState() == EPlayerState::PlayerRun) {
		if (GetResourceComponent()->bCanConsumeStamina(RunStaminaCost)) {
			//달리기 가능하니까 스테미나 소모하도록
			GetResourceComponent()->ConsumeStamina(RunStaminaCost);
		}
		else {
			//달리기 불가능하니까 스테이트 변경
			GetStateMachineComponent()->SetCurrentState(EPlayerState::PlayerWalk);
			TargetSpeed = WalkSpeed;
		}
	}
}

void AMyCharacter::CheckStaminaRecovery(float DeltaTime)
{
	if (!StateMachineComponent || !ResourceComponent)
		return;

	if (!bIsRoll && !bIsGuard && CanJump() && (StateMachineComponent->GetCurrentState() == EPlayerState::PlayerIdle || StateMachineComponent->GetCurrentState() == EPlayerState::PlayerWalk)) {
		if (ResourceComponent->GetCurrentStamina() < ResourceComponent->GetMaxStamina()) {
			ResourceComponent->ConsumeStamina(-StaminaRecoveryRate * DeltaTime);
		}
	}
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

void AMyCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource) {
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Hearing>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

AItemBase* AMyCharacter::GetQuickSlotItem()
{
	return QuickSlotItem ? QuickSlotItem : nullptr;
}

UWeaponBaseComponent* AMyCharacter::GetCurrentWeaponComponent()
{
	return CurrentWeaponComponent ? CurrentWeaponComponent : nullptr;
}

UBoxComponent* AMyCharacter::GetGuardComponent()
{
	return GuardComponent ? GuardComponent : nullptr;
}

UUserWidget* AMyCharacter::GetLockonWidgetInstance()
{
	return LockonWidgetInstance ? LockonWidgetInstance : nullptr;
}

UInventoryQuickSlotWidget* AMyCharacter::GetInventoryQuickSlotWidgetInstance()
{
	return InventoryQuickSlotWidgetInstance ? InventoryQuickSlotWidgetInstance : nullptr;
}

UResourceComponent* AMyCharacter::GetResourceComponent()
{
	return ResourceComponent ? ResourceComponent : nullptr;
}

UStateMachineComponent* AMyCharacter::GetStateMachineComponent()
{
	return StateMachineComponent ? StateMachineComponent : nullptr;
}

void AMyCharacter::SetPlayerInfo()
{
	if (CharacterData && ResourceComponent) {
		ResourceComponent->InitResource(CharacterData->MaxCharacterHP, CharacterData->MaxCharacterStamina, CharacterData->CharacterDamage);
	}
}

void AMyCharacter::SetNextSectionName(FName ChangeSectionName)
{
	NextSectionName = ChangeSectionName;
}

void AMyCharacter::SetQuickSlotItem(AItemBase* NewQuickSlotItem)
{
	if (NewQuickSlotItem) {
		QuickSlotItem = NewQuickSlotItem;
	}
}

void AMyCharacter::SetQuickSlotItemAmount(int32 NewAmount)
{
	QuickSlotItemAmount = NewAmount;
}

void AMyCharacter::SetQuickSlotItemID(int32 NewID)
{
	QuickSlotItemID = NewID;
}

void AMyCharacter::SetCurrentTalkNPC(ADialogueNPC* TalkNPC)
{
	CurrentTalkNPC = TalkNPC;
}
