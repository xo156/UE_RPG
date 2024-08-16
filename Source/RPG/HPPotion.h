// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "HPPotion.generated.h"

UCLASS()
class RPG_API AHPPotion : public AItemBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHPPotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Use();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value", meta = (AllowPrivateAccess = "true"))
	float HPRecoveryAmount;
};
