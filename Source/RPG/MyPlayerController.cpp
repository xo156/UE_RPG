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

//void AMyPlayerController::Attack(const FInputActionValue& Value) {
//	const float InputValue = Value.Get<float>();
//	if (GetCharacter() != nullptr) {
//		if (UAnimInstance* AnimInstance = GetCharacter()->GetMesh()->GetAnimInstance()) {
//			AnimInstance->Montage_Play(AttackMontage); //�ϴ� ���� ��� ���
//			if (bIsPossibleCombo(AnimInstance) && InputValue) { //�Է��� �ְ� �ε����� ����ϸ�
//				if (AttackMontage->CompositeSections.Num() > CurrentIndex) {//���� �ε����� �ȳ�ġ����
//					FName NextSectionName = AttackMontage->CompositeSections[++CurrentIndex].SectionName;
//					UE_LOG(LogTemp, Warning, TEXT("NextSectionName: %s"), *NextSectionName.ToString());
//					AnimInstance->Montage_JumpToSection(NextSectionName, AttackMontage);
//					AnimInstance->Montage_Play(AttackMontage);
//				}
//			}
//			else {
//				//�޺������� �� �ε����� �����ϱ�
//				AnimInstance->Montage_JumpToSection(AttackMontage->CompositeSections[0].SectionName, AttackMontage);
//				AnimInstance->Montage_Play(AttackMontage);
//			}
//			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Attack"));
//		}
//	}
//}

void AMyPlayerController::Attack(const FInputActionValue& Value) {
	const float InputValue = Value.Get<float>();
	if (GetCharacter() != nullptr) {
		if (UAnimInstance* AnimInstance = GetCharacter()->GetMesh()->GetAnimInstance()) {
			AnimInstance->Montage_Play(AttackMontage); //�ϴ� ���� ��� ���
			//TODO: �߰����� Ű �Է��� �ִ���
			int32 CurrentIndex = 0; //���� ������� ��Ÿ���� ���� �ε����� ���ϱ� ����
			if (bIsPossibleCombo(AnimInstance, CurrentIndex)) { //�Է��� �ְ� �ε����� ����ϸ�
				FName NextSectionName = AttackMontage->CompositeSections[++CurrentIndex].SectionName;
				UE_LOG(LogTemp, Warning, TEXT("NextSectionName: %s"), *NextSectionName.ToString());
				AnimInstance->Montage_JumpToSection(NextSectionName, AttackMontage);
				AnimInstance->Montage_Play(AttackMontage);
			}
			else {
				//�޺������� �� �ε����� �����ϱ�
				AnimInstance->Montage_JumpToSection(AttackMontage->CompositeSections[0].SectionName, AttackMontage);
				AnimInstance->Montage_Play(AttackMontage);
			}
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Attack"));
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

bool AMyPlayerController::bIsPossibleCombo(UAnimInstance* Anim, int32 CurrentIndex)
{
	if ((Anim->GetCurrentActiveMontage()->CompositeSections.Num() > 1) && 
		(CurrentIndex < Anim->GetCurrentActiveMontage()->CompositeSections.Num())) {
		return true;
	}
	return false;
}

