// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockonWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ULockonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetTarget(AActor* NewTarget);


protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	class UImage* LockonImage;

private:
	TWeakObjectPtr<AActor> TargetActor;
};
