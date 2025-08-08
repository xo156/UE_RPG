// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"
#include "MyCharacter.h"
#include "PlayerHUD.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "ItemBase.h"
#include "StaminaActorComponent.h"
#include "PlayerStateMachineComponent.h"

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

void AMyPlayerController::BeginPlay() {
	Super::BeginPlay();

	MyCharacter = Cast<AMyCharacter>(GetPawn());

	InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (InputSubsystem && NormalMappingContext) {
		//기본 IMC는 Normal이어야
		InputSubsystem->AddMappingContext(NormalMappingContext, 0);
	}
}

void AMyPlayerController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	MyCharacter = Cast<AMyCharacter>(InPawn);
}

void AMyPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnHancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		//노말
		EnHancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::TryMove);
		EnHancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AMyPlayerController::TryRunStart);
		EnHancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMyPlayerController::TryRunEnd);
		EnHancedInputComponent->BindAction(RollAction, ETriggerEvent::Completed, this, &AMyPlayerController::TryRoll);
		
		EnHancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::TryLook);
		
		EnHancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyPlayerController::TryJump);
		
		EnHancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Started, this, &AMyPlayerController::TryLightAttackStart);
		EnHancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &AMyPlayerController::TryHeavyAttakStart);

		EnHancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &AMyPlayerController::TryLockOnTarget);

		EnHancedInputComponent->BindAction(RootItemAction, ETriggerEvent::Started, this, &AMyPlayerController::TryInteract);

		EnHancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AMyPlayerController::TryOpenInventory);

		EnHancedInputComponent->BindAction(QuickSlotAction, ETriggerEvent::Started, this, &AMyPlayerController::TryUseQuickSlot);
	
		//인벤토리
		EnHancedInputComponent->BindAction(ConfirmAction, ETriggerEvent::Started, this, &AMyPlayerController::TryConfirm);
		EnHancedInputComponent->BindAction(NavigateAction, ETriggerEvent::Started, this, &AMyPlayerController::TryNavigate);
		EnHancedInputComponent->BindAction(CloseInventoryAction, ETriggerEvent::Started, this, &AMyPlayerController::TryCloseInventory);
	}
}

void AMyPlayerController::ChangeInputMappingContext(EIMCState NewIMC)
{
	switch (NewIMC)
	{
	case EIMCState::Normal:
		InputSubsystem->RemoveMappingContext(InventoryMappingContext);
		InputSubsystem->AddMappingContext(NormalMappingContext, 0);
		bShowMouseCursor = false;
		break;

	case EIMCState::Inventory:
		InputSubsystem->RemoveMappingContext(NormalMappingContext);
		InputSubsystem->AddMappingContext(InventoryMappingContext, 0);
		bShowMouseCursor = true;
		break;
	default:
		break;
	}

	CurrentIMC = NewIMC;
}

void AMyPlayerController::TryMove(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (GetCharacter() != nullptr) {
		if (!GetCharacter()->bIsRoll && 
			GetCharacter()->GetPlayerStateMachineComponent()->IsInAnyState({EPlayerState::Guard, EPlayerState::Idle, EPlayerState::Move})) {
			GetCharacter()->Move(InputValue);
		}
	}
}

void AMyPlayerController::TryRunStart()
{
	if (GetCharacter() != nullptr) {
		if (!GetCharacter()->bIsRoll && !GetCharacter()->bIsRun &&
			GetCharacter()->GetPlayerStateMachineComponent()->IsInAnyState({EPlayerState::Move})) {
			if (GetCharacter()->GetStaminaActorComponent()->CanConsumeStamina(GetCharacter()->RunStaminaCost)) {
				GetCharacter()->RunStart();
			}
		}
	}
}

void AMyPlayerController::TryRunEnd()
{
	if (GetCharacter() != nullptr) {
		if (GetCharacter()->bIsRun) {
			GetCharacter()->RunEnd();
		}
	}
}

void AMyPlayerController::TryRoll()
{
	if (GetCharacter() != nullptr) {
		if (GetCharacter()->CanJump() && !GetCharacter()->bIsRoll && !GetCharacter()->bIsRun &&
			!GetCharacter()->GetPlayerStateMachineComponent()->IsInAnyState({EPlayerState::LightAttack, EPlayerState::HeavyAttack, EPlayerState::Hit, EPlayerState::Parry})) {
			if (GetCharacter()->GetStaminaActorComponent()->CanConsumeStamina(GetCharacter()->RollStaminaCost)) {
				GetCharacter()->Roll();
			}
		}
	}
}

void AMyPlayerController::TryJump()
{
	if (GetCharacter() != nullptr) {
		if (GetCharacter()->CanJump() && !GetCharacter()->bIsRoll) {
			if (GetCharacter()->GetStaminaActorComponent()->CanConsumeStamina(GetCharacter()->JumpStaminaCost)) {
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

void AMyPlayerController::TryLightAttackStart(const FInputActionValue& Value)
{
	if (GetCharacter() != nullptr) {
		if (!GetCharacter()->bIsRoll && !GetCharacter()->bIsGuard && 
			GetCharacter()->GetPlayerStateMachineComponent()->IsInAnyState({EPlayerState::Guard, EPlayerState::Idle, EPlayerState::Move, EPlayerState::HeavyAttack, EPlayerState::LightAttack})) {
			if (GetCharacter()->GetStaminaActorComponent()->CanConsumeStamina(GetCharacter()->AttackStaminaCost)) {
				GetCharacter()->LightAttackStart();
			}
		}
	}
}

void AMyPlayerController::TryHeavyAttakStart(const FInputActionValue& Value)
{
	if (GetCharacter() != nullptr) {
		if (!GetCharacter()->bIsRoll && !GetCharacter()->bIsGuard &&
			GetCharacter()->GetPlayerStateMachineComponent()->IsInAnyState({EPlayerState::Guard, EPlayerState::Idle, EPlayerState::Move, EPlayerState::HeavyAttack, EPlayerState::LightAttack})) {
			if (GetCharacter()->GetStaminaActorComponent()->CanConsumeStamina(GetCharacter()->AttackStaminaCost)) {
				GetCharacter()->HeavyAttackChargeStart();
			}
		}
	}
}

void AMyPlayerController::TryGuard()
{
	if (GetCharacter() != nullptr) {
		if (!GetCharacter()->bIsRoll &&
			GetCharacter()->GetPlayerStateMachineComponent()->IsInAnyState({EPlayerState::Idle, EPlayerState::Move})) {
			//GetCharacter()->Guard();
		}
	}
}

void AMyPlayerController::TryLockOnTarget()
{
	if (GetCharacter() != nullptr) {
		GetCharacter()->LockOnTarget();
	}
}

void AMyPlayerController::TryInteract()
{
	if (GetCharacter() != nullptr) {
		if (!GetCharacter()->GetPlayerStateMachineComponent()->IsInAnyState({EPlayerState::LightAttack, EPlayerState::HeavyAttack, EPlayerState::Hit, EPlayerState::Dodge}) &&
			!GetCharacter()->bIsRoll) {
			GetCharacter()->Interact();
		}
	}
}

void AMyPlayerController::TryOpenInventory()
{	
	if (GetCharacter() != nullptr) {
		if (auto* InventoryComponent = GetCharacter()->GetInventoryComponent()) {
			if (auto* PlayerHUD = Cast<APlayerHUD>(GetHUD())) {
				PlayerHUD->OpenInventory(InventoryComponent);
				ChangeInputMappingContext(EIMCState::Inventory);
			}
		}
	}
}

void AMyPlayerController::TryUseQuickSlot()
{
	if (GetCharacter() != nullptr) {
		//GetCharacter()->UseQuickSlot();
	}
}

void AMyPlayerController::TrySelectQuickSlot()
{
	if (GetCharacter() != nullptr) {
		//GetCharacter()->SelectQuickSlot();
	}
}

void AMyPlayerController::TryOpenMainMenu()
{
	if (auto* PlayerHUD = Cast<APlayerHUD>(GetHUD())) {
		//TODO: PlayerHUD에서 메인 메뉴 위젯 열기
	}
}

void AMyPlayerController::TryConfirm()
{
	if (GetCharacter() != nullptr) {
		if (auto* InventoryComponent = GetCharacter()->GetInventoryComponent()) {
			if (auto* HUD = Cast<APlayerHUD>(GetHUD())) {
				if (auto* InventoryWidget = HUD->GetInventoryWidget()) {
					InventoryWidget->ConfirmFocusSlot();
				}
			}
		}
	}
}

void AMyPlayerController::TryNavigate(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (GetCharacter() != nullptr) {
		if (auto* InventoryComponent = GetCharacter()->GetInventoryComponent()) {
			if (auto* HUD = Cast<APlayerHUD>(GetHUD())) {
				if (auto* InventoryWidget = HUD->GetInventoryWidget()) {
					InventoryWidget->MoveFocus(InputValue);
				}
			}
		}
	}
}

void AMyPlayerController::TryCloseInventory()
{
	if (GetCharacter() != nullptr) {
		if (auto* InventoryComponent = GetCharacter()->GetInventoryComponent()) {
			if (auto* PlayerHUD = Cast<APlayerHUD>(GetHUD())) {
				PlayerHUD->CloseInventory(InventoryComponent);
				ChangeInputMappingContext(EIMCState::Normal);
			}
		}
	}
}
