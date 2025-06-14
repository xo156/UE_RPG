// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceComponent.h"

// Sets default values for this component's properties
UResourceComponent::UResourceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UResourceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UResourceComponent::InitResource(float MaxHealthAmount, float MaxStaminaAmount, float Damage, bool bUseStaminaResource)
{
	MaxHP = MaxHealthAmount;
	CurrentHP = MaxHP;

	MaxStamina = MaxStaminaAmount;
	CurrentStamina = MaxStamina;

	CurrentDamage = Damage;

	bUseStamina = bUseStaminaResource;
}

bool UResourceComponent::bCanConsumeStamina(float Consumption)
{
	return bUseStamina && (CurrentStamina >= Consumption);
}

bool UResourceComponent::bCanConsumeHealth(float Consumption)
{
	return CurrentHP >= Consumption;
}

void UResourceComponent::ConsumeHP(float HPConsumeption)
{
	CurrentHP = FMath::Clamp(CurrentHP - HPConsumeption, 0.0f, MaxHP);
	OnUIUpdated.Broadcast(CurrentHP, CurrentStamina);
}

void UResourceComponent::RecoverHP(float HPRecoveryAmount)
{
	CurrentHP = FMath::Clamp(CurrentHP + HPRecoveryAmount, 0.0f, MaxHP);
	OnUIUpdated.Broadcast(CurrentHP, CurrentStamina);
}

void UResourceComponent::ConsumeStamina(float StaminaComsumption)
{
	if (!bUseStamina)
		return;

	CurrentStamina = FMath::Clamp(CurrentStamina - StaminaComsumption, 0.0f, MaxStamina);
	OnUIUpdated.Broadcast(CurrentHP, CurrentStamina);
}

void UResourceComponent::RecoverStamina(float StaminaRecoveryAmount)
{
	if (!bUseStamina)
		return;

	CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRecoveryAmount, 0.0f, MaxStamina);
	OnUIUpdated.Broadcast(CurrentHP, CurrentStamina);
}

float UResourceComponent::GetCurrentHP() const
{
	return CurrentHP;
}

float UResourceComponent::GetMaxHP() const
{
	return MaxHP;
}

float UResourceComponent::GetHPRatio() const
{
	return CurrentHP / MaxHP;
}

float UResourceComponent::GetCurrentStamina() const
{
	return CurrentStamina;
}

float UResourceComponent::GetMaxStamina() const
{
	return MaxStamina;
}

float UResourceComponent::GetCurrentDamage() const
{
	return CurrentDamage;
}
