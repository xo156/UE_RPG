// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "MonsterWidgetComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UMonsterWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UMonsterWidgetComponent();

	void FaceToPlayer();
	void SetOwnerMonsterWidget();

	class UMonsterWidget* GetHealthWidget();

protected:
	virtual void BeginPlay() override;

private:
	class UMonsterWidget* HealthWidget;
	class AMonster* OwnerMonster;
};
