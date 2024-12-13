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
#include "Components/BoxComponent.h"
#include "InventoryWidget.h"
#include "InventoryItemAction.h"
#include "Kismet/KismetMathLibrary.h"
#include "Monster.h"
#include "DataTableGameInstance.h"
#include "InventoryQuickSlotWidget.h"
#include "DialogueNPC.h"
#include "ShowControlKeysWidget.h"

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
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay() {
	Super::BeginPlay();

	if (auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
		CameraShake = GameInstance->GetCameraShake();
		CharacterDataTable = GameInstance->GetCharacterDataTable();
		SetPlayerInfo();
	}

	if (WeaponComponent) {
		EquipWeapon(WeaponComponent);
	}

	SetupWidget();

	if (InventoryComponent) {
		InventoryComponent->CreateInventoryWidget();
	}

	bIsIdle = true;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//구조체
	CheckStaminaRecovery(DeltaTime);
	ChangeMoveSpeed(DeltaTime);

	if (GetVelocity().Y == 0.f && !bIsRoll)
		bIsIdle = true;

	//플레이어가 움직이는지 확인하기 위함
	PreviousLocation = CurrentLocation;
	CurrentLocation = GetActorLocation();

	//이전 위치와 현재 위치가 다른 경우 움직임이 있음
	bIsMove = (CurrentLocation != PreviousLocation);

	//락 온 중이면
	if (bIsLockon && CurrentTarget) {
		UpdateTargetVisibility();
		if (!IsTargetValid(CurrentTarget) || GetDistanceTo(CurrentTarget) > TargetRange) {
			ChangeTarget(nullptr);
			UpdateLockonEffect();
			return;
		}
		if (!bIsRoll) {
			bUseControllerRotationYaw = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
			//타겟 방향으로 바라보게 함
			if (bIsMove || bIsGuard) {
				UpdateLockOnCameraRotation();
				UpdateLockOnCameraPosition();
			}
		}
		else {
			FRotator CurrentRotation = GetActorRotation();
			SetActorRotation(CurrentRotation); // 현재 회전을 유지
		}
		if (LockonWidgetInstance)
			UpdateLockonEffect();
	}
	else {
		UpdateLockonEffect();
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void AMyCharacter::PlayAirboneMontage()
{
	if (GetCharacterMovement()->IsFalling()) {
		PlayAnimMontage(AirboneMontage);
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

	if (bHasEnoughHP(DamageAmount)) {
		//체력이 충분해서 데미지를 입을때
		ConsumeHPForAction(DamageAmount);
		UE_LOG(LogTemp, Log, TEXT("Player Damaged, CurrentHP: %f"), CurrentHP);
	}
	else {
		//체력이 없어서 죽을때
		ConsumeHPForAction(DamageAmount);
		UE_LOG(LogTemp, Log, TEXT("Player Die"));
	}

	return DamageAmount;
}

void AMyCharacter::Move(FVector2D InputValue)
{
	if (!bIsRoll) {
		const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		LastInputDirection = (ForwardDirection * InputValue.Y + RightDirection * InputValue.X).GetSafeNormal();

		AddMovementInput(ForwardDirection, InputValue.Y);
		AddMovementInput(RightDirection, InputValue.X);
	}
}

void AMyCharacter::RunStart()
{
	if (!bIsMove)
		return;

	if (bHasEnoughStamina(RunStaminaCost)) {
		bIsRun = true;
		TargetSpeed = RunSpeed;
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), RunLoudness, this);
	}
}

void AMyCharacter::RunEnd()
{
	bIsRun = false;
	TargetSpeed = WalkSpeed;
}

void AMyCharacter::Look(FVector2D InputValue)
{
	if (!InventoryComponent->bIsOpen) {
		AddControllerPitchInput(InputValue.Y);
		AddControllerYawInput(InputValue.X);
	}
}

void AMyCharacter::AttackStart()
{
	if (!bIsRoll && !bIsAttack && bHasEnoughStamina(AttackStaminaCost)) {
		bIsAttack = true;
		AttackExecute();
	}
}

void AMyCharacter::AttackExecute()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (CurrentWeaponComponent == nullptr) {
			bIsAttack = false;
			return;
		}
		//오버랩 액터 리스트 비우기
		CurrentWeaponComponent->GetRightHandWeaponInstance()->GetOverlapActors().Empty();
		CurrentWeaponComponent->GetLeftHandWeaponInstance()->GetOverlapActors().Empty();

		int32 SectionCount = CurrentWeaponComponent->GetSectionCount(CurrentWeaponComponent->AttackMontage);

		ConsumeStaminaForAction(AttackStaminaCost);
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), AttackLoudness, this);
		if (CurrentWeaponComponent->CurrentComboCount < SectionCount) {
			FString SectionName = "Combo" + FString::FromInt(CurrentWeaponComponent->CurrentComboCount);
			if (AnimInstance->Montage_IsPlaying(CurrentWeaponComponent->AttackMontage)) {
				AnimInstance->Montage_JumpToSection(FName(*SectionName), CurrentWeaponComponent->AttackMontage);
			}
			else {
				AnimInstance->Montage_Play(CurrentWeaponComponent->AttackMontage);
				AnimInstance->Montage_JumpToSection(FName(*SectionName), CurrentWeaponComponent->AttackMontage);
			}
			CurrentWeaponComponent->CurrentComboCount++;
		}
		else {
			CurrentWeaponComponent->CurrentComboCount = 0;
			FString SectionName = "Combo0";
			AnimInstance->Montage_Play(CurrentWeaponComponent->AttackMontage);
			AnimInstance->Montage_JumpToSection(FName(*SectionName), CurrentWeaponComponent->AttackMontage);
		}
	}
}

void AMyCharacter::SetComboAttackTimer()
{
	if (auto* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (AnimInstance->Montage_IsPlaying(CurrentWeaponComponent->AttackMontage)) {
			FName CurrentSectionName = AnimInstance->Montage_GetCurrentSection(CurrentWeaponComponent->AttackMontage);
			if (!CurrentSectionName.IsNone()) {
				float SectionLength = CurrentWeaponComponent->AttackMontage->GetSectionLength(CurrentWeaponComponent->AttackMontage->GetSectionIndex(CurrentSectionName));
				GetWorld()->GetTimerManager().SetTimer(ComboCheckTimerHandle, this, &AMyCharacter::StopComboAttackTimer, SectionLength, false);
				GetCurrentWeapon()->WaitComboTime = SectionLength;
			}
		}
	}
}

void AMyCharacter::StopComboAttackTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(ComboCheckTimerHandle);
	GetCurrentWeapon()->WaitComboTime = 0;
}

void AMyCharacter::AttackEnd()
{
	if (CurrentWeaponComponent) {
		CurrentWeaponComponent->CurrentComboCount = 0;
		CurrentWeaponComponent->GetRightHandWeaponInstance()->GetOverlapActors().Empty();
		CurrentWeaponComponent->GetLeftHandWeaponInstance()->GetOverlapActors().Empty();
	}
	bIsAttack = false;
}

void AMyCharacter::GuardUp()
{
	if (!InventoryComponent->bIsOpen) {
		if (bHasEnoughStamina(GuardStaminaCost)) {
			bIsGuard = true;
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
				GuardComponent->SetCollisionProfileName(TEXT("Pawn"));
				GuardComponent->SetVisibility(true);
				AnimInstance->Montage_Play(GuardMontage);
			}
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
	if (!InventoryComponent->bIsOpen) {
		if (bHasEnoughStamina(RollStaminaCost)) {
			if (!bIsAttack && CanJump() && !bIsRoll) {
				if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
					bIsRoll = true;
					ConsumeStaminaForAction(RollStaminaCost);

					if (!LastInputDirection.IsZero()) {
						SetActorRotation(LastInputDirection.Rotation());
					}

					bUseControllerRotationYaw = false;
					GetCharacterMovement()->bOrientRotationToMovement = true;
					AnimInstance->Montage_Play(RollMontage);
				}
			}
		}
	}
}

void AMyCharacter::LockOnTarget()
{
	if (bIsLockon) {
		//이미 락온된 상태에서는 새로운 타겟
		AActor* NewTarget = FindLockOnTarget();
		if (NewTarget) {
			ChangeTarget(NewTarget);
			CreateLockonEffect();
		}
		else {
			ChangeTarget(nullptr);
		}
	}
	else {
		AActor* NewTarget = FindLockOnTarget();
		if (NewTarget) {
			ChangeTarget(NewTarget);
			CreateLockonEffect();
		}
	}
}

AActor* AMyCharacter::FindLockOnTarget()
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 자기 자신 무시
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
	return NearestEnemy;
}

void AMyCharacter::UpdateTargetVisibility()
{
	if (bIsRoll)
		return;

	if (CurrentTarget) {
		//타겟이 항상 보이도록
		if (!IsTargetValid(CurrentTarget)) {
			ChangeTarget(nullptr); // 타겟이 유효하지 않으면
		}
	}
}

bool AMyCharacter::IsTargetValid(AActor* CheckTarget)
{
	if (CheckTarget == nullptr)
		return false;

	if (!CheckTarget->IsValidLowLevel() && CheckTarget->IsPendingKill())
		return false;

	if (GetDistanceTo(CheckTarget) > TargetRange)
		return false;

	if (auto* TargetMonster = Cast<AMonster>(CurrentTarget)) {
		if (TargetMonster->bIsMonsterDead) {
			ChangeTarget(nullptr);
		}
	}

	return true;
}

void AMyCharacter::ChangeTarget(AActor* NewTarget)
{
	if (NewTarget != CurrentTarget) {
		CurrentTarget = NewTarget;
		if (CurrentTarget == nullptr) {
			bIsLockon = false; 
			if (LockonWidgetInstance) {
				LockonWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else {
			bIsLockon = true;
			CreateLockonEffect();
		}
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
	if (CurrentTarget) {
		FVector DirectionToTarget = (CurrentTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FRotator TargetRotation = DirectionToTarget.Rotation();

		FRotator CurrentRotation = GetControlRotation();

		//시야각 제한
		float AngleDifference = FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw));

		if (AngleDifference > MaxTargetAngle) {
			//제한된 각도 내에서만 회전하도록 조정
			float AdjustedYaw = CurrentRotation.Yaw + FMath::Sign(TargetRotation.Yaw - CurrentRotation.Yaw) * MaxTargetAngle;
			CurrentRotation.Yaw = AdjustedYaw;
			if (auto* PlayerController = Cast<AMyPlayerController>(GetController())) {
				PlayerController->SetControlRotation(CurrentRotation);
			}
		}
		else {
			//제한이 없으면 자연스럽게 회전
			if (auto* PlayerController = Cast<AMyPlayerController>(GetController())) {
				PlayerController->SetControlRotation(TargetRotation);
			}
		}
	}
}

void AMyCharacter::UpdateLockOnCameraPosition()
{
	FRotator CurrentRotation = GetControlRotation();
	if (!bIsLockon) {
		if (auto* PlayerController = Cast<AMyPlayerController>(GetController())) {
			CurrentRotation.Pitch += 15.0f;
			CurrentRotation.Yaw -= 5.0f;
			PlayerController->SetControlRotation(GetActorRotation()); //원래 회전으로 되돌림
		}
	}
	else {
		//카메라를 우상단으로 살짝 이동
		CurrentRotation.Pitch -= 15.0f;
		CurrentRotation.Yaw += 5.0f;
		if (auto* PlayerController = Cast<AMyPlayerController>(GetController())) {
			PlayerController->SetControlRotation(CurrentRotation);
		}
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
	return InventoryComponent;
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

void AMyCharacter::TalkNPC()
{
	if (!bIsTalk && !CurrentTalkNPC) {
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this); // 자기 자신 무시
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
			FCollisionShape::MakeSphere(TalkRange),
			QueryParams
		);

		if (bHit) {
			for (FHitResult& Hit : HitResults) {
				AActor* HitActor = Hit.GetActor();
				if (HitActor) {
					if (HitActor->ActorHasTag(FName("NPC"))) {
						if (auto* DialogueNPC = Cast<ADialogueNPC>(HitActor)) {
							CurrentTalkNPC = DialogueNPC;
							DialogueNPC->ShowDialogues();
							bIsTalk = true;
							break;
						}
					}
				}
			}
		}
	}
	else {
		if (CurrentTalkNPC) {
			CurrentTalkNPC->GetDialogueComponent()->NextDialogue();
		}
	}
}

void AMyCharacter::ShowControlKeysWidget()
{
	if (ShowControlKeysWidgetInstance) {
		if (ShowControlKeysWidgetInstance->IsVisible()) {
			ShowControlKeysWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
		else {
			ShowControlKeysWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AMyCharacter::Close()
{
	if (InventoryComponent->InventoryWidget) {
		InventoryComponent->InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (ShowControlKeysWidgetInstance) {
		ShowControlKeysWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
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

UWeaponBaseComponent* AMyCharacter::GetCurrentWeapon() const
{
	return CurrentWeaponComponent ? CurrentWeaponComponent : nullptr;
}

void AMyCharacter::SetupWidget()
{
	if (PlayerWidgetClass) {
		PlayerWidgetInstance = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);
		if (PlayerWidgetInstance) {
			PlayerWidgetInstance->AddToViewport();
			PlayerWidgetInstance->UpdateHP(CurrentHP, MaxHP);
			PlayerWidgetInstance->UpdateStamina(CurrentStamina, MaxStamina);
		}
	}

	if (InventoryQuickSlotWidgetClass) {
		InventoryQuickSlotWidgetInstance = CreateWidget<UInventoryQuickSlotWidget>(GetWorld(), InventoryQuickSlotWidgetClass);
		if (InventoryQuickSlotWidgetInstance) {
			InventoryQuickSlotWidgetInstance->AddToViewport();
			InventoryQuickSlotWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (ShowControlKeysWidgetClass) {
		ShowControlKeysWidgetInstance = CreateWidget<UShowControlKeysWidget>(GetWorld(), ShowControlKeysWidgetClass);
		if (ShowControlKeysWidgetInstance) {
			ShowControlKeysWidgetInstance->AddToViewport();
			ShowControlKeysWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

float AMyCharacter::UseStamina(float StaminaCost)
{
	if (StaminaCost >= 0) {
		CurrentStamina = FMath::Max(CurrentStamina - StaminaCost, 0.0f);
	}
	else {
		CurrentStamina = FMath::Min(CurrentStamina + StaminaCost, MaxStamina);
	}
	return CurrentStamina;
}

float AMyCharacter::UseHP(float HPCost)
{
	if (HPCost >= 0) {
		CurrentHP = FMath::Max(CurrentHP - HPCost, 0.0f);
	}
	else {
		CurrentHP = FMath::Min(CurrentHP - HPCost, MaxHP);
	}
	return CurrentHP;
}

void AMyCharacter::ConsumeStaminaForAction(float StaminaCost)
{
	UseStamina(StaminaCost);
	OnPlayerUIUpdated.Broadcast(CurrentHP, CurrentStamina);
}

bool AMyCharacter::bHasEnoughStamina(float StaminaCost) const
{
	return CurrentStamina >= StaminaCost;
}

void AMyCharacter::ConsumeHPForAction(float HPCost)
{
	UseHP(HPCost);	
	OnPlayerUIUpdated.Broadcast(CurrentHP, CurrentStamina);
}

bool AMyCharacter::bHasEnoughHP(float HPCost) const
{
	return CurrentHP > HPCost;
}

void AMyCharacter::ChangeMoveSpeed(float DeltaTime)
{
	//현재 속도를 보간
	float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	float NewSpeed = FMath::Lerp(CurrentSpeed, TargetSpeed, 2.f * DeltaTime);
	if (NewSpeed < WalkSpeed)
		NewSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

	//달릴때만 스테미나 소모하도록
	if (bIsRun) {
		ConsumeStaminaForAction(RunStaminaCost);
	}
	//스테미나가 달리기에 부족하면
	if (!bHasEnoughStamina(RunStaminaCost)) {
		bIsRun = false;
		TargetSpeed = WalkSpeed;
	}
}

void AMyCharacter::CheckStaminaRecovery(float DeltaTime)
{
	if (!bIsAttack && !bIsRun && !bIsRoll) {
		if (CurrentStamina < MaxStamina) {
			TimeWithoutAction += DeltaTime;
			if (TimeWithoutAction >= 1.0f) {
				RecoveryStaminia(DeltaTime);
				TimeWithoutAction = 0.0f; //회복 후 초기화
			}
		}
	}
	else {
		TimeWithoutAction = 0.0f; //동작 수행 시 초기화
	}
}

void AMyCharacter::RecoveryStaminia(float DeltaTime)
{
	float StaminaRecoveryRate = 1000.0f;
	CurrentStamina = FMath::Min(CurrentStamina + (StaminaRecoveryRate * DeltaTime), MaxStamina);
	OnPlayerUIUpdated.Broadcast(CurrentHP, CurrentStamina);
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

void AMyCharacter::SetPlayerInfo()
{
	if (CharacterDataTable) {
		FCharacterData* CharacterData = CharacterDataTable->FindRow<FCharacterData>(FName(*FString::Printf(TEXT("%d"), PlayerCharacterType)), TEXT("SetPlayerInfo"));
		if (CharacterData) {
			MaxHP = CharacterData->MaxCharacterHP;
			CurrentHP = MaxHP;
			MaxStamina = CharacterData->MaxCharacterStamina;
			CurrentStamina = MaxStamina;
			Damage = CharacterData->CharacterDamage;
		}
	}
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
