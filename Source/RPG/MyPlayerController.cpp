// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Camera/CameraShakeBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"
#include "InventoryItemAction.h"
#include "InventoryTooltip.h"
#include "ItemBase.h"
#include "MyCharacter.h"
#include "WeaponBaseComponent.h"
#include "ResourceComponent.h"
#include "StateMachineComponent.h"

AMyPlayerController::AMyPlayerController()
{

}

AMyCharacter* AMyPlayerController::GetCharacter()
{
	if (MyCharacter == nullptr) {
		MyCharacter = Cast<AMyCharacter>(GetPawn());
	}
	return MyCharacter;
}

UResourceComponent* AMyPlayerController::GetResourceComponent()
{
	return GetCharacter() != nullptr ? GetCharacter()->GetResourceComponent() : nullptr;
}

UStateMachineComponent* AMyPlayerController::GetStateMachineComponent()
{
	return GetCharacter() != nullptr ? GetCharacter()->GetStateMachineComponent() : nullptr;
}

void AMyPlayerController::ShowTooltipAtMousePosition(UInventoryTooltip* TooltipWidget)
{
	if (!TooltipWidget)
		return;

	if (CurrentTooltip) {
		CurrentTooltip->RemoveFromParent();
		CurrentTooltip = nullptr;
	}

	CurrentTooltip = TooltipWidget;
	if (CurrentTooltip) {
		CurrentTooltip->AddToViewport();

		FVector2D MousePos;
		GetMousePosition(MousePos.X, MousePos.Y);

		FVector2D TooltipOffset(15.f, 15.f);
		CurrentTooltip->SetPositionInViewport(MousePos + TooltipOffset);
	}
}

void AMyPlayerController::HideTooltip()
{
	if (CurrentTooltip) {
		CurrentTooltip->RemoveFromParent();
		CurrentTooltip = nullptr;
	}
}

void AMyPlayerController::ShotItemActionMousePosition(UInventoryItemAction* ItemActionWidget)
{
	if (!ItemActionWidget)
		return;

	if (InventoryItemAction) {
		InventoryItemAction->RemoveFromParent();
		InventoryItemAction = nullptr;
	}

	InventoryItemAction = ItemActionWidget;
	if (InventoryItemAction) {
		InventoryItemAction->AddToViewport();

		FVector2D MousePos;
		GetMousePosition(MousePos.X, MousePos.Y);

		FVector2D TooltipOffset(15.f, 15.f);
		InventoryItemAction->SetPositionInViewport(MousePos + TooltipOffset);
	}
}

void AMyPlayerController::HideItemAction()
{
	if (InventoryItemAction) {
		InventoryItemAction->RemoveFromParent();
		InventoryItemAction = nullptr;
	}
}

void AMyPlayerController::BeginPlay() {
	Super::BeginPlay();

	MyCharacter = Cast<AMyCharacter>(GetPawn());
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
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
		EnHancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::TryMove);
		EnHancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AMyPlayerController::TryRunStart);
		EnHancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMyPlayerController::TryRunEnd);

		EnHancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::TryLook);
		
		EnHancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyPlayerController::TryJump);
		
		EnHancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Started, this, &AMyPlayerController::TryAttackStart);
				
		EnHancedInputComponent->BindAction(GuardAction, ETriggerEvent::Triggered, this, &AMyPlayerController::TryGuardUp);
		EnHancedInputComponent->BindAction(GuardAction, ETriggerEvent::Completed, this, &AMyPlayerController::TryGuardDown);
		
		EnHancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &AMyPlayerController::TryRoll);

		EnHancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &AMyPlayerController::TryLockOnTarget);

		EnHancedInputComponent->BindAction(RootItemAction, ETriggerEvent::Started, this, &AMyPlayerController::TryRootItem);

		EnHancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AMyPlayerController::TryOpenInventory);

		EnHancedInputComponent->BindAction(QuickSlotAction, ETriggerEvent::Started, this, &AMyPlayerController::TryQuickSlot);
		
		EnHancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMyPlayerController::TryInteract);
	}
}

void AMyPlayerController::TryMove(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (GetCharacter() != nullptr) {
		if (!GetCharacter()->bIsRoll && 
			GetStateMachineComponent()->GetCurrentState() != EPlayerState::PlayerAttack) {
			GetCharacter()->Move(InputValue);
		}
	}
}

void AMyPlayerController::TryRunStart()
{
	if (GetCharacter() != nullptr) {
		if (!GetCharacter()->bIsRoll && 
			GetStateMachineComponent()->GetCurrentState() == EPlayerState::PlayerWalk) {
			if (GetResourceComponent()->bCanConsumeStamina(GetCharacter()->RunStaminaCost)) {
				GetCharacter()->RunStart();
			}
		}
	}
}

void AMyPlayerController::TryRunEnd()
{
	if (GetCharacter() != nullptr) {
		if (GetStateMachineComponent()->GetCurrentState() == EPlayerState::PlayerRun) {
			GetCharacter()->RunEnd();
		}
	}
}

void AMyPlayerController::TryJump()
{
	if (GetCharacter() != nullptr) {
		if (GetCharacter()->CanJump()) {
			if (GetResourceComponent()->bCanConsumeStamina(GetCharacter()->JumpStaminaCost)) {
				GetCharacter()->Jump();
			}
		}
	}
}

void AMyPlayerController::TryLook(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (GetCharacter() != nullptr) {
		GetCharacter()->Look(InputValue);
	}
}

void AMyPlayerController::TryAttackStart(const FInputActionValue& Value)
{
	if (GetCharacter() != nullptr) {
		if (!GetCharacter()->bIsRoll) {
			if (GetResourceComponent()->bCanConsumeStamina(GetCharacter()->AttackStaminaCost)) {
				GetCharacter()->AttackStart();
			}
		}
	}
}

void AMyPlayerController::TryGuardUp()
{
	if (GetCharacter() != nullptr) {
		if (!GetCharacter()->bIsRoll && GetStateMachineComponent()->GetCurrentState() != EPlayerState::PlayerAttack) {
			if (GetResourceComponent()->bCanConsumeStamina(GetCharacter()->GuardStaminaCost)) {
				GetCharacter()->GuardUp();
			}
		}
	}
}

void AMyPlayerController::TryGuardDown()
{
	if (GetCharacter() != nullptr) {
		if (GetCharacter()->bIsGuard) {
			GetCharacter()->GuardDown();
		}
	}
}

void AMyPlayerController::TryRoll()
{
	if (GetCharacter() != nullptr) {
		if (GetCharacter()->CanJump() && !GetCharacter()->bIsRoll && GetStateMachineComponent()->GetCurrentState() != EPlayerState::PlayerAttack) {
			if (GetResourceComponent()->bCanConsumeStamina(GetCharacter()->RollStaminaCost)) {
				GetCharacter()->Roll();
			}
		}
	}
}

void AMyPlayerController::TryLockOnTarget()
{
	if (GetCharacter() != nullptr) {
		GetCharacter()->LockOnTarget();
	}
}

void AMyPlayerController::TryRootItem()
{
	if (GetCharacter() != nullptr) {
		if ((GetStateMachineComponent()->GetCurrentState() != EPlayerState::PlayerAttack) && !GetCharacter()->bIsRoll) {
			GetCharacter()->RootItem();
		}
	}
}

void AMyPlayerController::TryOpenInventory()
{
	if (GetCharacter() != nullptr) {
		GetCharacter()->OpenInventory();
	}
}

void AMyPlayerController::TryQuickSlot()
{
	if (GetCharacter() != nullptr) {
		GetCharacter()->QuickSlot();
	}
}

void AMyPlayerController::TryInteract()
{
	if (GetCharacter() != nullptr) {
		GetCharacter()->Interact();
	}
}

void AMyPlayerController::TryClose()
{
	if (GetCharacter() != nullptr){
		GetCharacter()->Close();
	}
}