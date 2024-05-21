// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Gauntlet.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AGauntlet : public AWeaponBase
{
	GENERATED_BODY()
	
	//함수들
public:
	// Sets default values for this actor's properties
	AGauntlet();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetMontageLength();

	//변수들
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AttackMontage1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AttackMontage2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AttackMontage3;

	TArray<float> MontageLength;
	FWeaponStruct MyWeapon;
};
