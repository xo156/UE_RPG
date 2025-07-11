// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HPActorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHPChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UHPActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHPActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitHP(float InMaxHP);
	void ConsumeHP(float Amount);
	void RecoverHP(float Amount);

	//확인
	bool CanConsumeHP(float Consumption) const;

	//getter
	float GetCurrentHP() const;
	float GetMaxHP() const;
	float GetHPRatio() const;

	//델리게이트
	FOnHPChanged OnHPChanged;

private:
	float MaxHP;
	float CurrentHP;
	float CurrentDamage;
};
