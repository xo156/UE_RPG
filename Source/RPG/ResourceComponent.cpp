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

void UResourceComponent::InitResource(float MaxHealthAmount, float MaxStaminaAmount, float Damage)
{
	MaxHealth = MaxHealthAmount;
	MaxStamina = MaxStaminaAmount;

	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;

	CurrentDamage = Damage;
}

bool UResourceComponent::bCanConsumeStamina(float Consumption)
{
	return CurrentStamina >= Consumption ? true : false;
}

bool UResourceComponent::bCanConsumeHealth(float Consumption)
{
	return CurrentHealth >= Consumption ? true : false;
}

void UResourceComponent::ConsumeHP(float HPConsumeption)
{
	UE_LOG(LogTemp, Log, TEXT("UResourceComponent::ConsumeHP"));

	CurrentHealth = FMath::Clamp(CurrentHealth - HPConsumeption, 0.0f, MaxHealth);
	OnPlayerUIUpdated.Broadcast(CurrentHealth, CurrentStamina);
}

void UResourceComponent::ConsumeStamina(float StaminaComsumption)
{
	UE_LOG(LogTemp, Log, TEXT("UResourceComponent::ConsumeStamina"));

	CurrentStamina = FMath::Clamp(CurrentStamina - StaminaComsumption, 0.0f, MaxStamina);
	OnPlayerUIUpdated.Broadcast(CurrentHealth, CurrentStamina);
}

float UResourceComponent::GetCurrentHealth()
{
	return CurrentHealth;
}

float UResourceComponent::GetMaxHealth()
{
	return MaxHealth;
}

float UResourceComponent::GetCurrentStamina()
{
	return CurrentStamina;
}

float UResourceComponent::GetMaxStamina()
{
	return MaxStamina;
}

float UResourceComponent::GetCurrentDamage()
{
	return CurrentDamage;
}
