// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWidget.h"
#include "HPActorComponent.h"
#include "Components/ProgressBar.h"


void UMonsterWidget::BindToComponents(UHPActorComponent* InHPActorComponent)
{
	HPActorComponent = InHPActorComponent;
	if (HPActorComponent) {
		HPActorComponent->OnHPChanged.AddDynamic(this, &UMonsterWidget::OnHPChanged);
		OnHPChanged();
	}
}

void UMonsterWidget::OnHPChanged()
{
	ProgressBar_HP->SetPercent(HPActorComponent->GetHPRatio());
}