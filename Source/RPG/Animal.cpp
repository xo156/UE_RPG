// Fill out your copyright notice in the Description page of Project Settings.


#include "Animal.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DataTableGameInstance.h"


// Sets default values
AAnimal::AAnimal()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkMoveSpeed;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	MountLocationComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MountLocation"));
	MountLocationComponent->SetVisibility(false);
	MountLocationComponent->SetupAttachment(RootComponent);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 65.f);
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
	ACharacter::Jump();
}

void AAnimal::Look(FVector2D InputValue)
{
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

void AAnimal::MountAnimal()
{
	/*if (auto* PlayerCharacter = Cast<AMyCharacter>(OwnerCharacter)) {
		PlayerCharacter->bIsRide = true;
		PlayerCharacter->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("MountPosition"));
		GetController()->Possess(this);
	}*/
}

void AAnimal::DisMountAnimal()
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(OwnerCharacter)) {
		PlayerCharacter->bIsRide = false;
		PlayerCharacter->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		PlayerCharacter->SetActorLocation(GetDisMountLocation(PlayerCharacter));
		GetController()->Possess(PlayerCharacter);
	}
}

FVector AAnimal::GetDisMountLocation(AMyCharacter* DisMountCharacter)
{
	if (DisMountCharacter) {
		FVector DisMountLocation;
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(DisMountCharacter);

		bool bFindLocation = false;

		while (!bFindLocation) {
			FVector RandomOffset = FMath::VRand() * FMath::FRandRange(0.f, 200.f);
			DisMountLocation = GetActorLocation() + RandomOffset;

			if (!GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), DisMountLocation, ECC_Visibility, QueryParams)) {
				bFindLocation = true;
				return DisMountLocation;
			}
		}
	}

	return FVector();
}

