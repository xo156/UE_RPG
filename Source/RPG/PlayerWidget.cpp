// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"
#include "HPActorComponent.h"
#include "StaminaActorComponent.h"
#include "Components/ProgressBar.h"
#include "MonsterWidget.h"

void UPlayerWidget::BindToComponents(UHPActorComponent* InHPActorComponent, UStaminaActorComponent* InStaminaActorComponent)
{
	HPActorComponent = InHPActorComponent;
	if (HPActorComponent) {
		HPActorComponent->OnHPChanged.AddDynamic(this, &UPlayerWidget::OnHPChanged);
		OnHPChanged();
	}

	StaminaActorComponent = InStaminaActorComponent;
	if (StaminaActorComponent) {
		StaminaActorComponent->OnStaminaChanged.AddDynamic(this, &UPlayerWidget::OnStaminaChanged);
		OnStaminaChanged();
	}
}

void UPlayerWidget::OnHPChanged()
{
	if (HPActorComponent && ProgressBar_HP) {
		ProgressBar_HP->SetPercent(HPActorComponent->GetHPRatio());
	}
}

void UPlayerWidget::OnStaminaChanged()
{
	if (StaminaActorComponent && ProgressBar_Stamina) {
		ProgressBar_Stamina->SetPercent(StaminaActorComponent->GetStaminaRatio());
	}
}
