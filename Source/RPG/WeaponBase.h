// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


UCLASS()
class RPG_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
//�Լ���
public:	
	// Sets default values for this actor's properties
	AWeaponBase(const class FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GetSectionCount(UAnimMontage* Montage);

	void SetOwnerCharacter(class AMyCharacter* NewOwner);
	void AttachMeshToCharacter();
	void OnEquip(const AWeaponBase* LastWeapon);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//������
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AttackMontage;

	//������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class USkeletalMeshComponent* WeaponRightHandMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UBoxComponent* WeaponRightCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponRightAttachPoint;

	//�޼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class USkeletalMeshComponent* WeaponLeftHandMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UBoxComponent* WeaponLeftCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponLeftAttachPoint;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	int32 CurrentComboCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	float WaitComboTime = 1.7f;

	class AMyCharacter* MyCharacter;
};
