// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaActorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UStaminaActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitStamina(float InMaxStamina);
	void ConsumeStamina(float Amount);
	void RecoverStamina(float Amount);

	//확인
	bool bCanConsumeStamina(float Consumption) const;

	//Getter
	float GetCurrentStamina() const;
	float GetMaxStamina() const;
	float GetStaminaRatio() const;

	//델리게이트
	FOnStaminaChanged OnStaminaChanged;

private:
	float MaxStamina;
	float CurrentStamina;
};
