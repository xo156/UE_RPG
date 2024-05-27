// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponBaseComponent.generated.h"


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UWeaponBaseComponent : public UActorComponent
{
	GENERATED_BODY()
	
//�Լ���
public:	
	// Sets default values for this actor's properties
	//UWeaponBaseComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int GetSectionCount(UAnimMontage* Montage);
	void SetOwnerCharacter(class AMyCharacter* NewOwner);
	void AttachMeshToCharacter();

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

	//�޼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class USkeletalMeshComponent* WeaponLeftHandMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UBoxComponent* WeaponLeftCollision;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	int32 CurrentComboCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	float WaitComboTime = 1.7f;

	class AMyCharacter* MyCharacter;

	
};
