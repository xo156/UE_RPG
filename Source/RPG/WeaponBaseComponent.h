// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponBaseComponent.generated.h"


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UWeaponBaseComponent : public UActorComponent
{
	GENERATED_BODY()
	
//함수들
public:	
	// Sets default values for this actor's properties
	UWeaponBaseComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int GetSectionCount(UAnimMontage* Montage);
	void SetOwnerCharacter(class AMyCharacter* NewOwner);
	void AttachToCharacter();

	class AWeapon* GetRightHandWeaponInstance() const;
	class AWeapon* GetLeftHandWeaponInstance() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//변수들
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AttackMontage;

	//공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	int32 CurrentComboCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	float WaitComboTime = 1.f;

	class AMyCharacter* OwnerCharacter;

	//무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AWeapon> RightHandWeapon;
	class AWeapon* RightHandWeaponInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AWeapon> LeftHandWeapon;
	class AWeapon* LeftHandWeaponInstance;
	
};
