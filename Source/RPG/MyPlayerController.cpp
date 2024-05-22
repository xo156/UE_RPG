// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponBase.h"

AMyPlayerController::AMyPlayerController() {

}

AMyCharacter* AMyPlayerController::GetCharacter() {
	if (MyCharacter == nullptr) {
		MyCharacter = Cast<AMyCharacter>(GetPawn());
	}

	return MyCharacter;
}

void AMyPlayerController::BeginPlay() {
	Super::BeginPlay();

	MyCharacter = Cast<AMyCharacter>(GetPawn());
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		SubSystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (WeaponClass) {
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
	}
}

void AMyPlayerController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	MyCharacter = Cast<AMyCharacter>(InPawn);
}

void AMyPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnHancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		EnHancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
		EnHancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Look);
		EnHancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyPlayerController::Jump);
		EnHancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMyPlayerController::Attack);
	}
}

void AMyPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (GetCharacter() != nullptr) {
		if (!bIsAttacking) {
			const FRotator YawRotation(0.f, GetCharacter()->GetControlRotation().Yaw, 0.f);
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			GetCharacter()->AddMovementInput(ForwardDirection, InputValue.Y);
			GetCharacter()->AddMovementInput(RightDirection, InputValue.X);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Move"));
		}
	}
}

void AMyPlayerController::Jump(const FInputActionValue& Value) {
	if (GetCharacter() != nullptr) {
		if (GetCharacter()->CanJump()) {
			GetCharacter()->Jump();
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("JumpUpMontage"));
		}
	}
}

void AMyPlayerController::Look(const FInputActionValue& Value) {
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (GetCharacter() != nullptr) {
		GetCharacter()->AddControllerPitchInput(InputValue.Y);
		GetCharacter()->AddControllerYawInput(InputValue.X);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Look"));
	}
}

void AMyPlayerController::Attack(const FInputActionValue& Value) {
	if (GetCharacter() != nullptr) {
		if (!bIsAttacking) {
			bIsAttacking = true;
			if (UAnimInstance* AnimInstance = GetCharacter()->GetMesh()->GetAnimInstance()) {
				if (CurrentWeapon == nullptr) {
					bIsAttacking = false;
					return;
				}
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Attack"));
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
				GetWorld()->GetTimerManager().SetTimer(ComboCheckTimerHandle, this, &AMyPlayerController::ResetAttackCount, CurrentWeapon->WaitComboTime, false);
			}
		}
	}
}

void AMyPlayerController::ResetAttackCount()
{
	if(CurrentWeapon)
		CurrentWeapon->CurrentComboCount = 0;
	bIsAttacking = false;
}

