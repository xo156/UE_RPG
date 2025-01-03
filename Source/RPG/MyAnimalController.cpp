// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimalController.h"
#include "Animal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AMyAnimalController::AMyAnimalController()
{
}

AAnimal* AMyAnimalController::GetAnimal()
{
	if (MyAnimal == nullptr) {
		MyAnimal = Cast<AAnimal>(GetPawn());
	}
	return MyAnimal;
}

void AMyAnimalController::BeginPlay()
{
	Super::BeginPlay();

	MyAnimal = Cast<AAnimal>(GetPawn());
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		SubSystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AMyAnimalController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	MyAnimal = Cast<AAnimal>(InPawn);
	UE_LOG(LogTemp, Log, TEXT("AMyAnimalController::OnPossess called for: %s"), *InPawn->GetName());
}

void AMyAnimalController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UE_LOG(LogTemp, Log, TEXT("AMyAnimalController::SetupInputComponent() called"));

	if (UEnhancedInputComponent* EnHancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		EnHancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyAnimalController::Move);
		EnHancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AMyAnimalController::RunStart);
		EnHancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMyAnimalController::RunEnd);

		EnHancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyAnimalController::Look);

		EnHancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyAnimalController::Jump);
		
		EnHancedInputComponent->BindAction(DisamountAction, ETriggerEvent::Started, this, &AMyAnimalController::Disamount);
	}
}

void AMyAnimalController::Move(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (GetAnimal() != nullptr) {
		UE_LOG(LogTemp, Log, TEXT("AMyAnimalController::Move"));
		GetAnimal()->Move(InputValue);
	}
}

void AMyAnimalController::RunStart()
{
	if (GetAnimal() != nullptr) {
		if (!GetAnimal()->bIsRun) {
			UE_LOG(LogTemp, Log, TEXT("AMyAnimalController::RunStart"));
			GetAnimal()->RunStart();
		}
	}
}

void AMyAnimalController::RunEnd()
{
	if (GetAnimal() != nullptr) {
		if (GetAnimal()->bIsRun) {
			UE_LOG(LogTemp, Log, TEXT("AMyAnimalController::RunEnd"));
			GetAnimal()->RunEnd();
		}
	}
}

void AMyAnimalController::Jump()
{
	if (GetAnimal() != nullptr) {
		if (GetAnimal()->CanJump()) {
			UE_LOG(LogTemp, Log, TEXT("AMyAnimalController::Jump"));
			GetAnimal()->Jump();
		}
	}
}

void AMyAnimalController::Look(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (GetAnimal() != nullptr) {
		UE_LOG(LogTemp, Log, TEXT("AMyAnimalController::Look"));
		GetAnimal()->Look(InputValue);
	}
}

void AMyAnimalController::Disamount()
{
	/*if (GetAnimal() != nullptr) {
		if (GetAnimal()->bIsMount) {
			UE_LOG(LogTemp, Log, TEXT("AMyAnimalController::Disamount"));
			GetAnimal()->DisMountAnimal();
		}
	}*/
}
