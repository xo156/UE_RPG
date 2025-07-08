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

	//캐릭터가 사용하기 위해
	void SetOwnerCharacter(class AMyCharacter* NewOwner);
	void AttachToCharacter();

	//무기
	class AWeapon* GetRightHandWeaponInstance() const;
	class AWeapon* GetLeftHandWeaponInstance() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//변수들
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* LightAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* HeavyAttackMontage;

	class AMyCharacter* OwnerCharacter;

	//무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AWeapon> RightHandWeapon;
	class AWeapon* RightHandWeaponInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AWeapon> LeftHandWeapon;
	class AWeapon* LeftHandWeaponInstance;
		
};
