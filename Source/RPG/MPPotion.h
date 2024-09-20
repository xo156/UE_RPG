// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "MPPotion.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AMPPotion : public AItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMPPotion();
	
	virtual void Use() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value", meta = (AllowPrivateAccess = "true"))
	float MPRecoveryAmount;

};
