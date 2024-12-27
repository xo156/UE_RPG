// Fill out your copyright notice in the Description page of Project Settings.


#include "Animal.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AAnimal::AAnimal()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = NormalMoveSpeed;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bDoCollisionTest = true;
	CameraBoom->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("VGCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	Tags.Add("Animal");
}

// Called when the game starts or when spawned
void AAnimal::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//동물이 움직이는지 확인하기 위함
	PreviousLocation = CurrentLocation;
	CurrentLocation = GetActorLocation();

	//이전 위치와 현재 위치가 다른 경우 움직임이 있음
	bIsMove = (CurrentLocation != PreviousLocation);
}

void AAnimal::Move(FVector2D InputValue)
{
	UE_LOG(LogTemp, Log, TEXT("AAnimal::Move(const FInputActionValue& Value)"));

	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(ForwardDirection, InputValue.Y);
	AddMovementInput(RightDirection, InputValue.X);
}

void AAnimal::RunStart()
{
	if (!bIsMove)
		return;

	if (!bIsRun) {
		bIsRun = true;
		GetCharacterMovement()->MaxWalkSpeed = RunMoveSpeed;
	}
}

void AAnimal::RunEnd()
{
	if (bIsRun) {
		bIsRun = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkMoveSpeed;
	}
}

void AAnimal::Jump()
{
	UE_LOG(LogTemp, Log, TEXT("AAnimal::Jump()"));
	if (CanJump()) {
		ACharacter::Jump();
	}
}

void AAnimal::Look(FVector2D InputValue)
{
	UE_LOG(LogTemp, Log, TEXT("AAnimal::Look(const FInputActionValue& Value)"));

	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void AAnimal::TaimAnimal(ACharacter* NewOwnerCharacter)
{
	if (NewOwnerCharacter) {
		OwnerCharacter = NewOwnerCharacter;
		if (auto* PlayerCharacter = Cast<AMyCharacter>(OwnerCharacter)) {
			PlayerCharacter->SetTaimmedAnimal(this);
		}
	}
}

void AAnimal::RideAnimal()
{
	if (OwnerCharacter) {
		OwnerCharacter->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("MountPos"));
		OwnerCharacter->GetController()->Possess(this);
		GetCharacterMovement()->MaxWalkSpeed = WalkMoveSpeed;
	}
}

void AAnimal::DropOutAnimal()
{
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	UnPossessed();
	GetCharacterMovement()->MaxWalkSpeed = NormalMoveSpeed;
}
