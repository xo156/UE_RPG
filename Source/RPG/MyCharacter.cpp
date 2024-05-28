// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "WeaponBaseComponent.h"
#include "MyPlayerController.h"

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

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 60.f);
	CameraBoom->bDoCollisionTest = true;
	CameraBoom->bUsePawnControlRotation = true;

	VGCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("VGCamera"));
	VGCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraBoom->bUsePawnControlRotation = false;
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
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, InputValue.Y);
	AddMovementInput(RightDirection, InputValue.X);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Move"));
}

void AMyCharacter::Look(FVector2D InputValue)
{
	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Look"));
}

void AMyCharacter::Attack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("attack call"));
	if (!bIsAttacking) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Attack1"));
		bIsAttacking = true;
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Attack2"));
			if (CurrentWeapon == nullptr) {
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Attack3"));
				bIsAttacking = false;
				return;
			}
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Attack4"));
			int32 SectionCount = CurrentWeapon->GetSectionCount(CurrentWeapon->AttackMontage);
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
}

void AMyCharacter::ResetAttackCount()
{
	if (CurrentWeapon)
		CurrentWeapon->CurrentComboCount = 0;
	bIsAttacking = false;
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

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay() {
	Super::BeginPlay();

	if (BareHand) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BareHand set"));
		EquipWeapon(BareHand);
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

