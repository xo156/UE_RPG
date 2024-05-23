// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "WeaponBase.h"
#include "MyPlayerController.h"

// Sets default values
AMyCharacter::AMyCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //이동하는 방향으로 캐릭터를 회전
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 60.f);
	CameraBoom->bDoCollisionTest = true;
	CameraBoom->bUsePawnControlRotation = true;

	VGCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("VGCamera"));
	VGCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);


}

void AMyCharacter::PlayAirboneMontage()
{
	if (GetCharacterMovement()->IsFalling()) {
		PlayAnimMontage(AirboneMontage);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("IsFalling()"));
	}
}

USkeletalMeshComponent* AMyCharacter::GetSpecificMesh() const
{
	return GetMesh();
}

FName AMyCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}

void AMyCharacter::EquipWeapon(AWeaponBase* Weapon)
{
	AMyPlayerController* MyController = Cast<AMyPlayerController>(GetController());
	if (Weapon) {
		SetCurrentWeapon(Weapon, MyController->CurrentWeapon);
	}
}

void AMyCharacter::AddWeapon(AWeaponBase* Weapon)
{
	if (Weapon)
		Inven.AddUnique(Weapon);
}

void AMyCharacter::SetCurrentWeapon(AWeaponBase* NewWeapon, AWeaponBase* LastWeapon)
{
	if (AWeaponBase* LocalLastWeapon = NULL) {
		LocalLastWeapon = LastWeapon;
	}

	if (NewWeapon) {
		NewWeapon->SetOwnerCharacter(this);
		NewWeapon->OnEquip(LastWeapon);
	}

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

