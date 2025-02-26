// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"
#include "Components/ProgressBar.h"
#include "MyCharacter.h"

void UPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto* PlayerCharacter = Cast<AMyCharacter>(GetOwningPlayerPawn())) {
		PlayerCharacter->GetResourceComponent()->OnPlayerUIUpdated.AddDynamic(this, &UPlayerWidget::OnPlayerResourceUpdate);
	}
}

void UPlayerWidget::UpdateHP(float CurrentHP, float MaxHP)
{
	if (HPProgressBar) {
		HPProgressBar->SetPercent(CurrentHP / MaxHP);
	}
}

void UPlayerWidget::UpdateStamina(float CurrentStamina, float MaxStamina)
{
	if (StaminaProgressBar) {
		StaminaProgressBar->SetPercent(CurrentStamina / MaxStamina);
	}
}

void UPlayerWidget::OnPlayerResourceUpdate(float NewHP, float NewStamina)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(GetOwningPlayerPawn())) {
		UpdateHP(NewHP, PlayerCharacter->GetResourceComponent()->GetMaxHealth());
		UpdateStamina(NewStamina, PlayerCharacter->GetResourceComponent()->GetMaxStamina());
	}
}
