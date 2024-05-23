// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"



UCLASS()
class RPG_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UCameraComponent* GetVGCameraComponent() const {
		return VGCameraComponent;
	}
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const {
		return CameraBoom;
	}

	void PlayAirboneMontage();

	class USkeletalMeshComponent* GetSpecificMesh() const;
	FName GetWeaponAttachPoint() const;
	void EquipWeapon(class AWeaponBase* Weapon);

	void AddWeapon(class AWeaponBase* Weapon);

	//Àåºñ ½ºÀ§Äª
	void SetCurrentWeapon(class AWeaponBase* NewWeapon, class AWeaponBase* LastWeapon);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AirboneMontage;

public:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FName WeaponAttachPoint;

	TArray<class AWeaponBase*> Inven;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* VGCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

};
