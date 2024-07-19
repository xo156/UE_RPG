// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWidget.h"
#include "Components/ProgressBar.h"
#include "Monster.h"

void UMonsterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto* Monster = Cast<AMonster>(GetOwningPlayerPawn())) {
		Monster->OnMonsterUIUpdated.AddDynamic(this, &UMonsterWidget::OnMonsterHPUpdate);
	}
}

void UMonsterWidget::UpdateHP(float CurrentHP, float MaxHP)
{
	if (HPProgressBar) {
		HPProgressBar->SetPercent(CurrentHP / MaxHP);
	}
}

void UMonsterWidget::OnMonsterHPUpdate(float NewHP)
{
	if (auto* Monster = Cast<AMonster>(GetOwningPlayerPawn())) {
		UpdateHP(NewHP, Monster->MonsterStatus.MaxMonsterHP);
	}
}
