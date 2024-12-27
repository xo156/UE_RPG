// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleController.h"
#include "Animal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AVehicleController::AVehicleController()
{
}

AAnimal* AVehicleController::GetVehicleAnimal()
{
	if (MyVehicleAnimal == nullptr) {
		MyVehicleAnimal = Cast<AAnimal>(GetPawn());
	}
	return MyVehicleAnimal;
}

void AVehicleController::BeginPlay()
{
	Super::BeginPlay();

	MyVehicleAnimal = Cast<AAnimal>(GetPawn());
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		SubSystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AVehicleController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	MyVehicleAnimal = Cast<AAnimal>(InPawn);
}

void AVehicleController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnHancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		EnHancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVehicleController::Move);
		EnHancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AVehicleController::RunStart);
		EnHancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AVehicleController::RunEnd);

		EnHancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVehicleController::Look);

		EnHancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AVehicleController::Jump);
	
		EnHancedInputComponent->BindAction(DropOutAnimalAction, ETriggerEvent::Started, this, &AVehicleController::DropOutAnimal);
	}
}

void AVehicleController::Move(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (GetVehicleAnimal() != nullptr) {
		GetVehicleAnimal()->Move(InputValue);
	}
}

void AVehicleController::RunStart()
{
	if (GetVehicleAnimal() != nullptr) {
		if (!GetVehicleAnimal()->bIsRun) {
			GetVehicleAnimal()->RunStart();
		}
	}
}

void AVehicleController::RunEnd()
{
	if (GetVehicleAnimal() != nullptr) {
		if (GetVehicleAnimal()->bIsRun) {
			GetVehicleAnimal()->RunEnd();
		}
	}
}

void AVehicleController::Jump()
{
	if (GetVehicleAnimal() != nullptr) {
		if (GetCharacter()->CanJump()) {
			GetVehicleAnimal()->Jump();
		}
	}
}

void AVehicleController::Look(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (GetVehicleAnimal() != nullptr) {
		GetVehicleAnimal()->Look(InputValue);
	}
}

void AVehicleController::DropOutAnimal()
{
	if (GetVehicleAnimal() != nullptr) {
		GetVehicleAnimal()->DropOutAnimal();
	}
}
