// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
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

// Sets default values
AMyCharacter::AMyCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //�̵��ϴ� �������� ĳ���͸� ȸ��
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

	VGCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("VGCamera"));
	VGCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	//����ü
	CharacterStatus.CurrentHP = 100.0f;
	CharacterStatus.MaxHP = 100.0f;
	CharacterStatus.CurrentStamina = 50.0f;
	CharacterStatus.MaxStamina = 50.0f;
	CharacterStatus.CurrentMP = 30.0f;
	CharacterStatus.MaxMP = 30.0f;
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
	if (!bIsDodging) {
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
		bIsRunning = true;
		TargetSpeed = RunSpeed;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Not Enough Stamina to Run"));
	}
}

void AMyCharacter::RunEnd()
{
	bIsRunning = false;
	TargetSpeed = WalkSpeed;
}

void AMyCharacter::Look(FVector2D InputValue)
{
	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void AMyCharacter::Attack()
{
	if (!bIsDodging && !bIsAttacking && bHasEnoughStamina(AttackStaminaCost)) {
		bIsAttacking = true;
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
			if (CurrentWeapon == nullptr) {
				bIsAttacking = false;
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
	bIsAttacking = false;
}

void AMyCharacter::Block()
{
	if (bHasEnoughStamina(BlockStaminaCost)) {
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
			//TODO: ������ ���� �������� �Ҹ��ϵ���
			//ConsumeStaminaForAction(BlockStaminaCost);
			//AnimInstance->Montage_Play(BlockMontage);
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Not Enough Stamina to Block"));
	}
}

void AMyCharacter::Dodge()
{
	if (bHasEnoughStamina(DodgeStaminaCost)) {
		if (!bIsAttacking && CanJump()) {
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

void AMyCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource) {
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

UWeaponBaseComponent* AMyCharacter::GetCurrentWeapon() const
{
	if (CurrentWeapon)
		return CurrentWeapon;
	return nullptr;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay() {
	Super::BeginPlay();

	//TODO: ������ �����Ҷ� ���������� ���߿��� �ٲ㺸��
	if (BareHand) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BareHand set"));
		EquipWeapon(BareHand);
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	CheckStaminaRecovery(DeltaTime);

	ChangeMoveSpeed(DeltaTime);
}

void AMyCharacter::ConsumeStaminaForAction(float StaminaCost)
{
	CharacterStatus.ConsumeStamina(StaminaCost);
}

bool AMyCharacter::bHasEnoughStamina(float StaminaCost) const
{
	return CharacterStatus.CurrentStamina >= StaminaCost;
}

void AMyCharacter::ConsumeMPForAction(float MPCost)
{
	CharacterStatus.ConsumeMP(MPCost);
}

bool AMyCharacter::bHasEnoughMP(float MPCost) const
{
	return CharacterStatus.CurrentMP >= MPCost;
}

void AMyCharacter::ChangeMoveSpeed(float DeltaTime)
{
	//���� �ӵ��� ����
	float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	float NewSpeed = FMath::Lerp(CurrentSpeed, TargetSpeed, 1.f * DeltaTime);
	if (NewSpeed < WalkSpeed)
		NewSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

	//�޸����� ���׹̳� �Ҹ��ϵ���
	if (bIsRunning) {
		ConsumeStaminaForAction(RunStaminaCost);
	}
	//���׹̳��� �޸��⿡ �����ϸ�
	if (!bHasEnoughStamina(RunStaminaCost)) {
		bIsRunning = false;
		TargetSpeed = WalkSpeed;
	}
}

void AMyCharacter::CheckStaminaRecovery(float DeltaTime)
{
	if (!bIsAttacking && !bIsRunning && !bIsDodging) {
		if (CharacterStatus.CurrentStamina < CharacterStatus.MaxStamina) {
			TimeWithoutAction += DeltaTime;
			if (TimeWithoutAction >= 1.0f) {
				RecoveryStaminia(DeltaTime);
				TimeWithoutAction = 0.0f; // ȸ�� �� �ʱ�ȭ
			}
		}
	}
	else {
		TimeWithoutAction = 0.0f; // ���� ���� �� �ʱ�ȭ
	}
}

void AMyCharacter::RecoveryStaminia(float DeltaTime)
{
	float StaminaRecoveryRate = 1000.0f; // �� �ʴ� ȸ���Ǵ� ���¹̳� ��
	CharacterStatus.CurrentStamina = FMath::Min(CharacterStatus.CurrentStamina + (StaminaRecoveryRate * DeltaTime), CharacterStatus.MaxStamina);
	UE_LOG(LogTemp, Warning, TEXT("RecoveryStamina: %f"), CharacterStatus.CurrentStamina)
}
