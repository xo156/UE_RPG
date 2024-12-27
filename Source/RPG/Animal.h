// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animal.generated.h"

UCLASS()
class RPG_API AAnimal : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAnimal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Move(FVector2D InputValue);
	void RunStart();
	void RunEnd();
	void Jump();
	void Look(FVector2D InputValue);

	void TaimAnimal(ACharacter* NewOwnerCharacter);
	void RideAnimal();
	void DropOutAnimal();

	bool bIsMove = false;
	bool bIsRun = false;

private:
	////컨트롤러
	//class AMyPlayerController* PlayerCharacterController;
	//class AVehicleController* VehicleAnimalController;

	//탑승 및 하차
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ride", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RideLocationComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ride", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DropOutLocationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	FVector PreviousLocation;
	FVector CurrentLocation;

	float NormalMoveSpeed = 300.f;
	float WalkMoveSpeed = 600.f;
	float RunMoveSpeed = 1000.f;
	ACharacter* OwnerCharacter;
};
