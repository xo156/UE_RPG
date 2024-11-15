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

	//구조체
	CharacterStatus.MaxHP = 100.f;
	CharacterStatus.CurrentHP = CharacterStatus.MaxHP;
	CharacterStatus.MaxStamina = 50.f;
	CharacterStatus.CurrentStamina = CharacterStatus.MaxStamina;
	CharacterStatus.Damage = 20.f;

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

	if (WeaponComponent) {
		EquipWeapon(WeaponComponent);
	}

	SetupWidget();

	if (InventoryComponent) {
		InventoryComponent->CreateInventoryWidget();
	}

	bIsIdle = true;

	if (auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
		ItemTable = GameInstance->GetItemTable();
		CameraShake = GameInstance->GetCameraShake();
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

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
			if (bIsMove || bIsAttack || bIsGuard) {
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTarget->GetActorLocation());
				LookAtRotation.Pitch -= TargetHeightOffset;
				GetController()->SetControlRotation(LookAtRotation);
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
		UE_LOG(LogTemp, Log, TEXT("Player Damaged, CurrentHP: %f"), CharacterStatus.CurrentHP);
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
	UE_LOG(LogTemp, Log, TEXT("AMyCharacter::AttackEnd()"));
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
		ChangeTarget(NewTarget);
		CreateLockonEffect();
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

void AMyCharacter::RootItem()
{
	if (InventoryComponent) {
		TArray<ADropItem*> ItemsToRemove;
		if (OverlapItems.Num() > 0) {
			for (ADropItem* Item : OverlapItems) {
				if (Item) {
					UE_LOG(LogTemp, Log, TEXT("Adding item with ID: %d to inventory"), Item->DropItemData.ItemID);
					bool bAdded = InventoryComponent->TryAddItem(Item);
					if (!bAdded) {
						UE_LOG(LogTemp, Warning, TEXT("Failed to add item with ID: %d to inventory"), Item->DropItemData.ItemID);
					}
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
				UE_LOG(LogTemp, Log, TEXT("Item added to overlap list."));
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
				UE_LOG(LogTemp, Log, TEXT("Item removed from overlap list."));
			}
		}
	}
}

void AMyCharacter::QuickSlot()
{
	if (QuickSlotItem && InventoryComponent && QuickSlotItemAmount > 0) {
		QuickSlotItem->Use();
		InventoryComponent->RemoveItem(QuickSlotItemID, 1);
		SetQuickSlotItemAmount(QuickSlotItemAmount - 1);
		if (QuickSlotItemAmount <= 0) {
			QuickSlotItemAmount = 0;
		}
		InventoryComponent->InventoryWidget->UpdateInventoryWidget(InventoryComponent);
	}
}

void AMyCharacter::Close()
{
	if (InventoryComponent->InventoryWidget) {
		InventoryComponent->InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		//TODO:나중에 여기에 게임 종료까지 이어지도록
	}
}

void AMyCharacter::EquipWeapon(TSubclassOf<class UWeaponBaseComponent> WeaponBaseComponentClass)
{
	if (WeaponBaseComponentClass) {
		if (CurrentWeaponComponent) {
			if (ItemTable) {
				static const FString ContextString(TEXT("Item Table"));
				for (const FName& RowName : ItemTable->GetRowNames()) {
					FItemData* ItemData = ItemTable->FindRow<FItemData>(RowName, ContextString);
					if (ItemData) {
						for (const auto& ItemClass : ItemData->ItemClass) {
							if (ItemClass && CurrentWeaponComponent->GetRightHandWeaponInstance()->IsA(ItemClass)) {
								if (auto NowEquipWeapon = GetWorld()->SpawnActor<ADropItem>(ADropItem::StaticClass())) {
									FDropItemData NowEquipWeaponData;
									NowEquipWeaponData.Amount = 1;
									NowEquipWeaponData.bCounterble = false;
									NowEquipWeaponData.ItemID = ItemData->ItemID;
									NowEquipWeapon->SetDropItem(NowEquipWeaponData);
									if (InventoryComponent) {
										InventoryComponent->TryAddItem(NowEquipWeapon);
									}
								}
								break;
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
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Now EquipWeapon : %s"), *CurrentWeaponComponent->GetName()));
		}
	}
}

UWeaponBaseComponent* AMyCharacter::GetCurrentWeapon() const
{
	if (CurrentWeaponComponent)
		return CurrentWeaponComponent;
	return nullptr;
}

void AMyCharacter::SetupWidget()
{
	if (PlayerWidgetClass) {
		PlayerWidgetInstance = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);
		if (PlayerWidgetInstance) {
			PlayerWidgetInstance->AddToViewport();
			PlayerWidgetInstance->UpdateHP(CharacterStatus.CurrentHP, CharacterStatus.MaxHP);
			PlayerWidgetInstance->UpdateStamina(CharacterStatus.CurrentStamina, CharacterStatus.MaxStamina);
		}
	}

	if (InventoryQuickSlotWidgetClass) {
		InventoryQuickSlotWidgetInstance = CreateWidget<UInventoryQuickSlotWidget>(GetWorld(), InventoryQuickSlotWidgetClass);
		if (InventoryQuickSlotWidgetInstance) {
			InventoryQuickSlotWidgetInstance->AddToViewport();
		}
	}
}

void AMyCharacter::ConsumeStaminaForAction(float StaminaCost)
{
	CharacterStatus.UseStamina(StaminaCost);
	OnPlayerUIUpdated.Broadcast(CharacterStatus.CurrentHP, CharacterStatus.CurrentStamina);
}

bool AMyCharacter::bHasEnoughStamina(float StaminaCost) const
{
	return CharacterStatus.CurrentStamina >= StaminaCost;
}

void AMyCharacter::ConsumeHPForAction(float HPCost)
{
	CharacterStatus.UseHP(HPCost);	
	OnPlayerUIUpdated.Broadcast(CharacterStatus.CurrentHP, CharacterStatus.CurrentStamina);
}

bool AMyCharacter::bHasEnoughHP(float HPCost) const
{
	return CharacterStatus.CurrentHP > HPCost;
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
		if (CharacterStatus.CurrentStamina < CharacterStatus.MaxStamina) {
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
	CharacterStatus.CurrentStamina = FMath::Min(CharacterStatus.CurrentStamina + (StaminaRecoveryRate * DeltaTime), CharacterStatus.MaxStamina);
	OnPlayerUIUpdated.Broadcast(CharacterStatus.CurrentHP, CharacterStatus.CurrentStamina);
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

void AMyCharacter::TEST()
{	
	if (QuickSlotItem) {
		UE_LOG(LogTemp, Log, TEXT("QuickSlotItem : %s"), *QuickSlotItem->GetName());
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("TEST"));
}

AActor* AMyCharacter::GetCurrentTarget()
{
	if (CurrentTarget)
		return CurrentTarget;
	return nullptr;
}

AActor* AMyCharacter::GetPrevLockOnTarget()
{
	if (PrevLockOnTarget)
		return PrevLockOnTarget;
	return nullptr;
}

float AMyCharacter::GetTargetHeightOffset()
{
	return TargetHeightOffset;
}

AItemBase* AMyCharacter::GetQuickSlotItem()
{
	if (QuickSlotItem)
		return QuickSlotItem;
	return nullptr;
}

UWeaponBaseComponent* AMyCharacter::GetCurrentWeaponComponent()
{
	if (CurrentWeaponComponent)
		return CurrentWeaponComponent;
	return nullptr;
}

UBoxComponent* AMyCharacter::GetGuardComponent()
{
	if (GuardComponent)
		return GuardComponent;
	return nullptr;
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
