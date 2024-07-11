// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Monster.h"

void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto* Monster = Cast<AMonster>(GetOwningPlayerPawn())) {
		Monster->OnUIUpdated.AddDynamic(this, &UHealthBarWidget::OnMonsterHPUpdate);
	}
}

void UHealthBarWidget::UpdateHP(float CurrentHP, float MaxHP)
{
	if (HPProgressBar) {
		HPProgressBar->SetPercent(CurrentHP / MaxHP);
	}
}

void UHealthBarWidget::OnMonsterHPUpdate(float NewHP)
{
	if (auto* Monster = Cast<AMonster>(GetOwningPlayerPawn())) {
		UpdateHP(NewHP, Monster->MonsterStatus.MaxMonsterHP);
	}
}
