// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponBaseComponent.h"

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
}

void AMyPlayerController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	MyCharacter = Cast<AMyCharacter>(InPawn);
}

void AMyPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnHancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		EnHancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
		EnHancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AMyPlayerController::RunStart);
		EnHancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMyPlayerController::RunEnd);

		EnHancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Look);
		EnHancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyPlayerController::Jump);
		EnHancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMyPlayerController::Attack);
	}
}

void AMyPlayerController::AccessCharacterStatus()
{
	if (GetCharacter() != nullptr) {
		float CurrentHealth = MyCharacter->CharacterStatus.CurrentHP;
		float MaxHealth = MyCharacter->CharacterStatus.MaxHP;
		float CurrentStamina = MyCharacter->CharacterStatus.CurrentStamina;
		float MaxStamina = MyCharacter->CharacterStatus.MaxStamina;
		float CurrentMP = MyCharacter->CharacterStatus.CurrentMP;
		float MaxMP = MyCharacter->CharacterStatus.MaxMP;
	}
}

void AMyPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (GetCharacter() != nullptr) {
		if (!GetCharacter()->bIsAttacking) {
			GetCharacter()->Move(InputValue);
		}
	}
}

void AMyPlayerController::RunStart()
{
	if (GetCharacter() != nullptr) {
		if (!GetCharacter()->bIsRunning) {
			GetCharacter()->RunStart();
		}
	}
}

void AMyPlayerController::RunEnd()
{
	if (GetCharacter() != nullptr) {
		if (GetCharacter()->bIsRunning) {
			GetCharacter()->RunEnd();
		}
	}
}

void AMyPlayerController::Jump(const FInputActionValue& Value) {
	if (GetCharacter() != nullptr) {
		if (GetCharacter()->CanJump() && GetCharacter()->bHasEnoughStamina(GetCharacter()->JumpStaminaCost)) {
			GetCharacter()->ConsumeStaminaForAction(GetCharacter()->JumpStaminaCost);
			GetCharacter()->Jump();
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("JumpUpMontage"));
		}
	}
}

void AMyPlayerController::Look(const FInputActionValue& Value) {
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (GetCharacter() != nullptr) {
		GetCharacter()->Look(InputValue);
	}
}

void AMyPlayerController::Attack(const FInputActionValue& Value) {
	if (GetCharacter() != nullptr) {
		GetCharacter()->Attack();
	}
}
