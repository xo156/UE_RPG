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
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable, Category = "Status")
	void UpdateHP(float CurrentHP, float MaxHP);

	UFUNCTION()
	void OnMonsterHPUpdate(float NewHP);

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPProgressBar;
};
