// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


UCLASS()
class RPG_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
//함수들
public:	
	// Sets default values for this actor's properties
	AWeaponBase(const class FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GetSectionCount(UAnimMontage* Montage);

	void SetOwnerCharacter(class AMyCharacter* NewOwner);
	void AttachMeshToCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//변수들
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AttackMontage;

	//오른손
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class USkeletalMeshComponent* WeaponRightHandMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UBoxComponent* WeaponRightCollision;

	//왼손
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class USkeletalMeshComponent* WeaponLeftHandMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UBoxComponent* WeaponLeftCollision;

	//공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	int32 CurrentComboCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	float WaitComboTime = 1.7f;

	class AMyCharacter* MyCharacter;
};
