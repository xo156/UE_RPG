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
#include "PlayerStatusUserWidget.h"

// Sets default values
AMyCharacter::AMyCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //이동하는 방향으로 캐릭터를 회전
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 60.f);
	CameraBoom->bDoCollisionTest = true;
	CameraBoom->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("VGCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	//구조체
	CharacterStatus.CurrentHP = 100.0f;
	CharacterStatus.MaxHP = 100.0f;
	CharacterStatus.CurrentStamina = 50.0f;
	CharacterStatus.MaxStamina = 50.0f;
	CharacterStatus.CurrentMP = 30.0f;
	CharacterStatus.MaxMP = 30.0f;

	//위젯
	PlayerStatusWidgetClass = UPlayerStatusUserWidget::StaticClass();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	CheckStaminaRecovery(DeltaTime);

	ChangeMoveSpeed(DeltaTime);

	//TODO: 락온 완성하고나서 함수로 하자
	if (bIsLockOnTarget && LockedOnTarget) {
		FVector TargetLocation = LockedOnTarget->GetActorLocation();
		FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
		FRotator NewControlRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		NewControlRotation.Pitch = 0.0f;  // 캐릭터의 Pitch 회전은 고정
		GetController()->SetControlRotation(NewControlRotation);
	}
}

void AMyCharacter::PlayAirboneMontage()
{
	if (GetCharacterMovement()->IsFalling()) {
		PlayAnimMontage(AirboneMontage);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("IsFalling()"));
	}
}

void AMyCharacter::Move(FVector2D InputValue)
{
	if (!bIsDodge) {
		const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, InputValue.Y);
		AddMovementInput(RightDirection, InputValue.X);
	}
}

void AMyCharacter::RunStart()
{
	if (bHasEnoughStamina(RunStaminaCost)) {
		bIsRun = true;
		TargetSpeed = RunSpeed;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Not Enough Stamina to Run"));
	}
}

void AMyCharacter::RunEnd()
{
	bIsRun = false;
	TargetSpeed = WalkSpeed;
}

void AMyCharacter::Look(FVector2D InputValue)
{
	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void AMyCharacter::Attack()
{
	if (!bIsDodge && !bIsAttack && bHasEnoughStamina(AttackStaminaCost)) {
		bIsAttack = true;
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
			if (CurrentWeapon == nullptr) {
				bIsAttack = false;
				return;
			}
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Attack"));
			int32 SectionCount = CurrentWeapon->GetSectionCount(CurrentWeapon->AttackMontage);
			ConsumeStaminaForAction(AttackStaminaCost);
			if (CurrentWeapon->CurrentComboCount < SectionCount) {
				FString SectionName = "Combo" + FString::FromInt(CurrentWeapon->CurrentComboCount);
				if (AnimInstance->Montage_IsPlaying(CurrentWeapon->AttackMontage)) {
					AnimInstance->Montage_JumpToSection(FName(*SectionName), CurrentWeapon->AttackMontage);
				}
				else {
					AnimInstance->Montage_Play(CurrentWeapon->AttackMontage);
					AnimInstance->Montage_JumpToSection(FName(*SectionName), CurrentWeapon->AttackMontage);
				}
				CurrentWeapon->CurrentComboCount++;
			}
			else {
				CurrentWeapon->CurrentComboCount = 0;
				FString SectionName = "Combo0";
				AnimInstance->Montage_Play(CurrentWeapon->AttackMontage);
				AnimInstance->Montage_JumpToSection(FName(*SectionName), CurrentWeapon->AttackMontage);
			}
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("CurrentComboCount: %d"), CurrentWeapon->CurrentComboCount));
			GetWorld()->GetTimerManager().SetTimer(ComboCheckTimerHandle, this, &AMyCharacter::ResetAttackCount, CurrentWeapon->WaitComboTime, false);
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Not Enough Stamina to Attack"));
	}
}

void AMyCharacter::ResetAttackCount()
{
	if (CurrentWeapon)
		CurrentWeapon->CurrentComboCount = 0;
	bIsAttack = false;
}

void AMyCharacter::Guard()
{
	if (bHasEnoughStamina(BlockStaminaCost)) {
		bIsGuard = true;
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
			/*AnimInstance->Montage_Play(BlockMontage, 1.0f);
			FName StartSection = TEXT("GuardStart");
			AnimInstance->Montage_JumpToSection(StartSection, BlockMontage);*/
		}
	}
}

void AMyCharacter::Dodge()
{
	if (bHasEnoughStamina(DodgeStaminaCost)) {
		if (!bIsAttack && CanJump()) {
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
				ConsumeStaminaForAction(DodgeStaminaCost);

				//AnimInstance->Montage_Play(DodgeMontage);
			}
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Not Enough Stamina to Dodge"));
	}
}

void AMyCharacter::LockOnTarget()
{
	if (bIsLockOnTarget) {
		// 이미 락온된 상태에서는 새로운 타겟을 찾습니다.
		AActor* NewTarget = FindNewTarget();
		if (NewTarget) {
			LockedOnTarget = NewTarget;
			// 디버그용으로 락온된 타겟을 표시
			DrawDebugSphere(GetWorld(), LockedOnTarget->GetActorLocation(), 50.0f, 12, FColor::Red, false, 5.0f);
		}
		else {
			// 새로운 타겟을 찾지 못하면 락온을 해제합니다.
			UnLockOnTarget();
		}
	}
	else {
		FVector CameraLocation = CameraComponent->GetComponentLocation();
		FVector CameraForwardVector = CameraComponent->GetForwardVector();

		// 부채꼴 범위 내의 모든 액터를 검색합니다.
		AActor* NearestTarget = nullptr;
		float NearestDistance = LockOnConeRadius;

		TArray<AActor*> PotentialTargets;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), PotentialTargets);

		for (AActor* Actor : PotentialTargets) {
			//플레이어 자신을 무시
			if (Actor != this) {
				FVector DirectionToActor = (Actor->GetActorLocation() - CameraLocation).GetSafeNormal();
				float DistanceToActor = FVector::Dist(CameraLocation, Actor->GetActorLocation());

				// 카메라의 전방 벡터와 타겟까지의 벡터 간의 각도를 계산
				float AngleToActor = FMath::Acos(FVector::DotProduct(CameraForwardVector, DirectionToActor));
				AngleToActor = FMath::RadiansToDegrees(AngleToActor);

				if (AngleToActor <= LockOnConeAngle && DistanceToActor < NearestDistance) {
					NearestTarget = Actor;
					NearestDistance = DistanceToActor;
				}
			}
		}
		if (NearestTarget) {
			LockedOnTarget = NearestTarget;
			bIsLockOnTarget = true;
			// 디버그용으로 락온된 타겟을 표시
			DrawDebugSphere(GetWorld(), LockedOnTarget->GetActorLocation(), 50.0f, 12, FColor::Red, false, 5.0f);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Lock On"));
}

void AMyCharacter::UnLockOnTarget()
{
	LockedOnTarget = nullptr;
	bIsLockOnTarget = false;
}

AActor* AMyCharacter::FindNewTarget() const
{
	if (!LockedOnTarget) return nullptr;

	FVector CameraLocation = CameraComponent->GetComponentLocation();
	FVector CameraForwardVector = CameraComponent->GetForwardVector();

	// 현재 타겟의 위치를 가져옵니다.
	FVector CurrentTargetLocation = LockedOnTarget->GetActorLocation();

	// 특정 반경 내의 모든 액터를 검색합니다.
	TArray<AActor*> PotentialTargets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), PotentialTargets);

	AActor* NearestTarget = nullptr;
	float NearestDistance = LockOnConeRadius;

	for (AActor* Actor : PotentialTargets) {
		//플레이어 자신과 현재 타겟을 무시	
		if (Actor != this) {
			FVector DirectionToActor = (Actor->GetActorLocation() - CameraLocation).GetSafeNormal();
			float DistanceToActor = FVector::Dist(CurrentTargetLocation, Actor->GetActorLocation());

			// 카메라의 전방 벡터와 타겟까지의 벡터 간의 각도를 계산
			float AngleToActor = FMath::Acos(FVector::DotProduct(CameraForwardVector, DirectionToActor));
			AngleToActor = FMath::RadiansToDegrees(AngleToActor);

			if (AngleToActor <= LockOnConeAngle && DistanceToActor < NearestDistance) {
				NearestTarget = Actor;
				NearestDistance = DistanceToActor;
			}
		}
	}

	return NearestTarget;
}

void AMyCharacter::EquipWeapon(TSubclassOf<class UWeaponBaseComponent> WeaponClass)
{
	if (WeaponClass) {
		CurrentWeapon = NewObject<UWeaponBaseComponent>(this, WeaponClass);
		if (CurrentWeapon) {
			CurrentWeapon->SetOwnerCharacter(this);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Now EquipWeapon : %s"), *CurrentWeapon->GetName()));
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to spawn weapon"));
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("WeaponClass is not set"));
	}
}

UWeaponBaseComponent* AMyCharacter::GetCurrentWeapon() const
{
	if (CurrentWeapon)
		return CurrentWeapon;
	return nullptr;
}

void AMyCharacter::SetupWidget()
{
	if (PlayerStatusWidgetClass) {
		PlayerStatusWidgetInstance = CreateWidget<UPlayerStatusUserWidget>(GetWorld(), PlayerStatusWidgetClass);
		if (PlayerStatusWidgetInstance) {
			PlayerStatusWidgetInstance->AddToViewport();
			PlayerStatusWidgetInstance->UpdateHP(CharacterStatus.CurrentHP, CharacterStatus.MaxHP);
			PlayerStatusWidgetInstance->UpdateMP(CharacterStatus.CurrentMP, CharacterStatus.MaxMP);
			PlayerStatusWidgetInstance->UpdateStamina(CharacterStatus.CurrentStamina, CharacterStatus.MaxStamina);
		}
	}
}

void AMyCharacter::ConsumeStaminaForAction(float StaminaCost)
{
	CharacterStatus.UseStamina(StaminaCost);
	OnUIUpdated.Broadcast(CharacterStatus.CurrentHP, CharacterStatus.CurrentMP, CharacterStatus.CurrentStamina);
}

bool AMyCharacter::bHasEnoughStamina(float StaminaCost) const
{
	return CharacterStatus.CurrentStamina >= StaminaCost;
}

void AMyCharacter::ConsumeMPForAction(float MPCost)
{
	CharacterStatus.UseMP(MPCost);
	OnUIUpdated.Broadcast(CharacterStatus.CurrentHP, CharacterStatus.CurrentMP, CharacterStatus.CurrentStamina);
}

bool AMyCharacter::bHasEnoughMP(float MPCost) const
{
	return CharacterStatus.CurrentMP >= MPCost;
}

void AMyCharacter::ConsumeHPForAction(float HPCost)
{
	CharacterStatus.UseHP(HPCost);	
	OnUIUpdated.Broadcast(CharacterStatus.CurrentHP, CharacterStatus.CurrentMP, CharacterStatus.CurrentStamina);
}

bool AMyCharacter::bHasEnoughHP(float HPCost)
{
	return CharacterStatus.CurrentHP >= HPCost;
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
	if (!bIsAttack && !bIsRun && !bIsDodge) {
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
	OnUIUpdated.Broadcast(CharacterStatus.CurrentHP, CharacterStatus.CurrentMP, CharacterStatus.CurrentStamina);
	UE_LOG(LogTemp, Warning, TEXT("RecoveryStamina: %f"), CharacterStatus.CurrentStamina)
}

void AMyCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource) {
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

void AMyCharacter::TESTSTATUS()
{
	CharacterStatus.UseStamina(50.f);
	CharacterStatus.UseMP(3.f);
	CharacterStatus.UseHP(5.f);

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("TESTSTATUS"));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay() {
	Super::BeginPlay();

	//TODO: 지금은 시작할때 장착하지만 나중에는 바꿔보자
	if (BareHand) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BareHand set"));
		EquipWeapon(BareHand);
	}
	
	SetupWidget();
}
