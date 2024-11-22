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
	class UMonsterWidget* GetMonsterWidgetClass();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess="true"))
	class UMonsterWidget* MonsterWidget;

	class AMonster* OwnerMonster;
};
