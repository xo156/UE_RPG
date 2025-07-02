// Fill out your copyright notice in the Description page of Project Settings.


#include "HPActorComponent.h"

// Sets default values for this component's properties
UHPActorComponent::UHPActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHPActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHPActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHPActorComponent::InitHP(float InMaxHP)
{
	MaxHP = InMaxHP;
	CurrentHP = MaxHP;
	OnHPChanged.Broadcast(CurrentHP);

	CurrentDamage = CurrentDamage;
}

void UHPActorComponent::ConsumeHP(float Amount)
{
	CurrentHP = FMath::Clamp(CurrentHP - Amount, 0.0f, MaxHP);
	OnHPChanged.Broadcast(CurrentHP);
}

void UHPActorComponent::RecoverHP(float Amount)
{
	CurrentHP = FMath::Clamp(CurrentHP + Amount, 0.0f, MaxHP);
	OnHPChanged.Broadcast(CurrentHP);
}

bool UHPActorComponent::bCanConsumeHP(float Consumption) const
{
	return CurrentHP >= Consumption;
}

float UHPActorComponent::GetCurrentHP() const
{
	return CurrentHP;
}

float UHPActorComponent::GetMaxHP() const
{
	return MaxHP;
}

float UHPActorComponent::GetHPRatio() const
{
	return MaxHP > 0 ? CurrentHP / MaxHP : 0.0f;
}
