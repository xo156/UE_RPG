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
	UWeaponBaseComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//��Ÿ��
	int GetSectionCount(UAnimMontage* Montage);

	//ĳ���Ͱ� ����ϱ� ����
	void SetOwnerCharacter(class AMyCharacter* NewOwner);
	void AttachToCharacter();

	//����
	class AWeapon* GetRightHandWeaponInstance() const;
	class AWeapon* GetLeftHandWeaponInstance() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//������
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AttackMontage;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	int32 CurrentComboCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	float WaitComboTime = 1.f;

	class AMyCharacter* OwnerCharacter;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AWeapon> RightHandWeapon;
	class AWeapon* RightHandWeaponInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AWeapon> LeftHandWeapon;
	class AWeapon* LeftHandWeaponInstance;
	
};
