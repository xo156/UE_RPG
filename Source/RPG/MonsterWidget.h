// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UMonsterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void BindToComponents(class UHPActorComponent* InHPActorComponent);

	UFUNCTION()
	void OnHPChanged();

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_HP;

	class UHPActorComponent* HPActorComponent;
};
