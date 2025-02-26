// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerUIUpdated, float, NewHP, float, NewStamina);
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
	void InitResource(float MaxHealthAmount, float MaxStaminaAmount, float Damage);

	//확인
	bool bCanConsumeStamina(float Consumption);
	bool bCanConsumeHealth(float Consumption);

	//setter
	void ConsumeHP(float HPConsumeption);
	void ConsumeStamina(float StaminaComsumption);
	
	//getter
	float GetCurrentHealth();
	float GetMaxHealth();
	float GetCurrentStamina();
	float GetMaxStamina();
	float GetCurrentDamage();

	//델리게이트
	FOnPlayerUIUpdated OnPlayerUIUpdated;

private:
	//플레이어
	float MaxHealth;
	float MaxStamina;
	float CurrentHealth;
	float CurrentStamina;
	float CurrentDamage;
};
