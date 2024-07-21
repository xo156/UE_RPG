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

// Sets default values
AMyCharacter::AMyCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //이동하는 방향으로 캐릭터를 회전
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
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
	CharacterStatus.Damage = 20.f;
	CharacterStatus.CurrentMoney = 0;
	CharacterStatus.MaxMoney = 10000;

	//위젯
	PlayerStatusWidgetClass = UPlayerWidget::StaticClass();
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

// Called every frame
void AMyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	CheckStaminaRecovery(DeltaTime);

	ChangeMoveSpeed(DeltaTime);

	//플레이어가 움직이는지 확인하기 위함
	PreviousLocation = CurrentLocation;
	CurrentLocation = GetActorLocation();

	// 이전 위치와 현재 위치가 다른 경우 움직임이 있음
	bIsMove = (CurrentLocation != PreviousLocation);

	//락온 시 카메라
	if (bIsLockOnTarget && LockedOnTarget) {
		float TargetDist = FVector::Dist(LockedOnTarget->GetActorLocation(), GetActorLocation());
		if (TargetDist > MaxLockOnDist) {
			UnLockOnTarget();
		}
		else {
			LockOnCamera(DeltaTime);
		}
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

		bIsMove = true;
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

void AMyCharacter::AttackStart()
{
	if (!bIsDodge && !bIsAttack && bHasEnoughStamina(AttackStaminaCost)) {
		bIsAttack = true;
		AttackExecute();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Not Enough to Attack"));
	}
}

void AMyCharacter::AttackExecute()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (CurrentWeapon == nullptr) {
			bIsAttack = false;
			return;
		}		
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Attack"));
		int32 SectionCount = CurrentWeapon->GetSectionCount(CurrentWeapon->AttackMontage);
		ConsumeStaminaForAction(AttackStaminaCost);
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), AttackLoudness, this);
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
		GetWorld()->GetTimerManager().SetTimer(ComboCheckTimerHandle, this, &AMyCharacter::AttackEnd, CurrentWeapon->WaitComboTime, false);
	}
}

void AMyCharacter::AttackEnd()
{
	if (CurrentWeapon) {
		CurrentWeapon->CurrentComboCount = 0;
	}
	bIsAttack = false;
}

void AMyCharacter::Guard()
{
	if (bHasEnoughStamina(GuardStaminaCost)) {
		bIsGuard = true;
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
			//ConsumeStaminaForAction(GuardStaminaCost);
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
				AnimInstance->Montage_Play(DodgeMontage);
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
		//이미 락온된 상태에서는 새로운 타겟
		if (AActor* NewTarget = FindNewTarget()) {
			LockedOnTarget = NewTarget;
			DrawDebugSphere(GetWorld(), LockedOnTarget->GetActorLocation(), 150.0f, 12, FColor::Red, false, 5.0f);
		}
		else {
			//새로운 타겟을 찾지 못하면 락온을 해제
			UnLockOnTarget();
		}
	}
	else {
		if (AActor* NewTarget = FindNewTarget()) {
			LockedOnTarget = NewTarget;
			DrawDebugSphere(GetWorld(), LockedOnTarget->GetActorLocation(), 150.0f, 12, FColor::Red, false, 5.0f);
		}
	}
}

AActor* AMyCharacter::FindNewTarget()
{
	FVector CameraLocation = CameraComponent->GetComponentLocation();
	FVector CameraForwardVector = CameraComponent->GetForwardVector();

	//범위 내의 모든 액터를 검색
	AActor* NearestTarget = nullptr;
	float NearestDistance = LockOnConeRadius;

	TArray<AActor*> PotentialTargets;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), FName(TEXT("Enemy")), PotentialTargets);

	//이미 락온중인거는 거르도록
	for (AActor* Target : PotentialTargets) {
		if (LockedOnTarget == Target) {
			PotentialTargets.Remove(Target);
		}
	}

	for (AActor* Target : PotentialTargets) {
		FVector DirectionToActor = (Target->GetActorLocation() - CameraLocation).GetSafeNormal();
		float DistToActor = FVector::Dist(CameraLocation, Target->GetActorLocation());

		//카메라의 전방 벡터와 타겟까지의 벡터 간의 각도를 계산
		float AngleToTarget = FMath::Acos(FVector::DotProduct(CameraForwardVector, DirectionToActor));
		AngleToTarget = FMath::RadiansToDegrees(AngleToTarget);

		if (AngleToTarget <= LockOnConeAngle && DistToActor < NearestDistance) {
			NearestTarget = Target;
			NearestDistance = DistToActor;
		}
	}
	if (NearestTarget) {
		bIsLockOnTarget = true;
	}
	return NearestTarget;
}

void AMyCharacter::LockOnCamera(float DeltaTime)
{
	FVector TargetLocation = LockedOnTarget->GetActorLocation();
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewRotation.Pitch = 0.0f;  // 캐릭터의 Pitch 회전은 고정
	SetActorRotation(NewRotation);
	float CurrentCameraBoomOffsetZ = CameraBoom->SocketOffset.Z;
	float NewOffsetZ = FMath::Lerp(CurrentCameraBoomOffsetZ, 90.f, 5.f * DeltaTime);
	if (NewOffsetZ <= 90.f) {
		CameraBoom->SocketOffset = FVector(0.f, 0.f, NewOffsetZ);
	}
}

void AMyCharacter::UnLockOnTarget()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("UnLockOnTarget"));
	LockedOnTarget = nullptr;
	bIsLockOnTarget = false;

	CameraBoom->SocketOffset = FVector(0.f, 0.f, 60.f);
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
		PlayerStatusWidgetInstance = CreateWidget<UPlayerWidget>(GetWorld(), PlayerStatusWidgetClass);
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
	OnPlayerUIUpdated.Broadcast(CharacterStatus.CurrentHP, CharacterStatus.CurrentMP, CharacterStatus.CurrentStamina);
}

bool AMyCharacter::bHasEnoughStamina(float StaminaCost) const
{
	return CharacterStatus.CurrentStamina >= StaminaCost;
}

void AMyCharacter::ConsumeMPForAction(float MPCost)
{
	CharacterStatus.UseMP(MPCost);
	OnPlayerUIUpdated.Broadcast(CharacterStatus.CurrentHP, CharacterStatus.CurrentMP, CharacterStatus.CurrentStamina);
}

bool AMyCharacter::bHasEnoughMP(float MPCost) const
{
	return CharacterStatus.CurrentMP >= MPCost;
}

void AMyCharacter::ConsumeHPForAction(float HPCost)
{
	CharacterStatus.UseHP(HPCost);	
	OnPlayerUIUpdated.Broadcast(CharacterStatus.CurrentHP, CharacterStatus.CurrentMP, CharacterStatus.CurrentStamina);
}

bool AMyCharacter::bHasEnoughHP(float HPCost) const
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
	OnPlayerUIUpdated.Broadcast(CharacterStatus.CurrentHP, CharacterStatus.CurrentMP, CharacterStatus.CurrentStamina);
	UE_LOG(LogTemp, Warning, TEXT("RecoveryStamina: %f"), CharacterStatus.CurrentStamina)
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
	CharacterStatus.UseStamina(50.f);
	CharacterStatus.UseMP(3.f);
	CharacterStatus.UseHP(5.f);
	

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("TEST"));
}
