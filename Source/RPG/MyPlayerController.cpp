// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	/*SetMontage();
	SetMontageLength();*/
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
		const FRotator YawRotation(0.f, GetCharacter()->GetControlRotation().Yaw, 0.f);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		GetCharacter()->AddMovementInput(ForwardDirection, InputValue.Y);
		GetCharacter()->AddMovementInput(RightDirection, InputValue.X);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Move"));
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
	const float InputValue = Value.Get<float>();
	if (GetCharacter() != nullptr) {
		if (!MyCharacter->bAttacking) {
			if (UAnimInstance* AnimInstance = GetCharacter()->GetMesh()->GetAnimInstance()) {
				MyCharacter->bAttacking = true;
				AnimInstance->Montage_Play(AttackMontage1); //1타 모션 출력
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Attack"));
			}
		}
		else {
			//TODO: 추가적인 키 입력이 일정 시간 안으로 있는지
			/*GetWorldTimerManager().SetTimer(ComboCheck, this, &AMyPlayerController::CheckComboTime, 1.f, false);
			AnimInstance->Montage_Play(AttackMontage2);
			GetWorldTimerManager().SetTimer(ComboCheck, this, &AMyPlayerController::CheckComboTime, 1.f, false);
			AnimInstance->Montage_Play(AttackMontage3);*/
		}
	}
}

void AMyPlayerController::PickUPItem(const FInputActionValue& Value)
{
	if (GetCharacter() != nullptr) {
		//줍는 아이템이 유효한가, 아이템의 종류가 무엇인가
			//무기이면 캐릭터에 장비 시킨다
	}
}

