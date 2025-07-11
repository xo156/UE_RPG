// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaActorComponent.h"

// Sets default values for this component's properties
UStaminaActorComponent::UStaminaActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UStaminaActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStaminaActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStaminaActorComponent::InitStamina(float InMaxStamina)
{
	MaxStamina = InMaxStamina;
	CurrentStamina = MaxStamina;
	OnStaminaChanged.Broadcast();
}

void UStaminaActorComponent::ConsumeStamina(float Amount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina - Amount, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast();
}

void UStaminaActorComponent::RecoverStamina(float Amount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + Amount, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast();
}

bool UStaminaActorComponent::CanConsumeStamina(float Consumption) const
{
	return CurrentStamina >= Consumption;
}

float UStaminaActorComponent::GetCurrentStamina() const
{
	return CurrentStamina;
}

float UStaminaActorComponent::GetMaxStamina() const
{
	return MaxStamina;
}

float UStaminaActorComponent::GetStaminaRatio() const
{
	return MaxStamina > 0.f ? CurrentStamina / MaxStamina : 0.f;
}

