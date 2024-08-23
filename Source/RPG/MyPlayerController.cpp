// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponBaseComponent.h"
#include "ItemBase.h"

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
		
		EnHancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMyPlayerController::AttackStart);
		
		EnHancedInputComponent->BindAction(BlockAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Guard);
		
		EnHancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &AMyPlayerController::Dodge);
	
		EnHancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &AMyPlayerController::LockOnTarget);
		
		EnHancedInputComponent->BindAction(RootItemAction, ETriggerEvent::Started, this, &AMyPlayerController::RootItem);
		
		EnHancedInputComponent->BindAction(TESTSTATUSAction, ETriggerEvent::Started, this, &AMyPlayerController::TEST);
	}
	
}

void AMyPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (GetCharacter() != nullptr) {
		if (!GetCharacter()->bIsAttack) {
			GetCharacter()->Move(InputValue);
		}
	}
}

void AMyPlayerController::RunStart()
{
	if (GetCharacter() != nullptr) {
		if (!GetCharacter()->bIsRun) {
			GetCharacter()->RunStart();
		}
	}
}

void AMyPlayerController::RunEnd()
{
	if (GetCharacter() != nullptr) {
		if (GetCharacter()->bIsRun) {
			GetCharacter()->RunEnd();
		}
	}
}

void AMyPlayerController::Jump() {
	if (GetCharacter() != nullptr) {
		if (GetCharacter()->CanJump() && GetCharacter()->bHasEnoughStamina(GetCharacter()->JumpStaminaCost)) {
			GetCharacter()->ConsumeStaminaForAction(GetCharacter()->JumpStaminaCost);
			GetCharacter()->Jump();
		}
	}
}

void AMyPlayerController::Look(const FInputActionValue& Value) {
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (GetCharacter() != nullptr) {
		GetCharacter()->Look(InputValue);
	}
}

void AMyPlayerController::AttackStart(const FInputActionValue& Value) {
	if (GetCharacter() != nullptr) {
		GetCharacter()->AttackStart();
	}
}

void AMyPlayerController::Guard()
{
	if (GetCharacter() != nullptr) {
		GetCharacter()->Guard();
	}
}

void AMyPlayerController::Dodge()
{
	if (GetCharacter() != nullptr) {
		GetCharacter()->Dodge();
	}
}

void AMyPlayerController::LockOnTarget()
{
	if (GetCharacter() != nullptr) {
		GetCharacter()->LockOnTarget();
	}
}

void AMyPlayerController::RootItem()
{
	if (GetCharacter() != nullptr) {
		GetCharacter()->RootItem();
	}
}

void AMyPlayerController::OpenInventory()
{
	if (GetCharacter() != nullptr) {
		GetCharacter()->OpenInventory();
	}
}

void AMyPlayerController::TEST()
{
	if (GetCharacter() != nullptr) {
		GetCharacter()->TEST();
	}
}
