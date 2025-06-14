// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUIUpdated, float, NewHP, float, NewStamina);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UResourceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//초기화
	void InitResource(float MaxHealthAmount, float MaxStaminaAmount, float Damage, bool bUseStaminaResource);

	//확인
	bool bCanConsumeStamina(float Consumption);
	bool bCanConsumeHealth(float Consumption);

	//조정
	void ConsumeHP(float HPConsumeption);
	void RecoverHP(float HPRecoveryAmount);
	void ConsumeStamina(float StaminaComsumption);
	void RecoverStamina(float StaminaRecoveryAmount);
	
	//getter
	float GetCurrentHP() const;
	float GetMaxHP() const;
	float GetHPRatio() const;
	float GetCurrentStamina() const;
	float GetMaxStamina() const;
	float GetCurrentDamage() const;

	//델리게이트
	FOnUIUpdated OnUIUpdated;

private:
	float MaxHP;
	float CurrentHP;
	float MaxStamina;
	float CurrentStamina;
	float CurrentDamage;

	bool bUseStamina = false;
};
