// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusUserWidget.h"
#include "Components/ProgressBar.h"
#include "MyCharacter.h"

void UPlayerStatusUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto* PlayerCharacter = Cast<AMyCharacter>(GetOwningPlayerPawn())) {
		PlayerCharacter->OnUIUpdated.AddDynamic(this, &UPlayerStatusUserWidget::OnPlayerStatusUswerWidgetUpdate);
	}
}

void UPlayerStatusUserWidget::UpdateHP(float CurrentHP, float MaxHP)
{
	if (HPProgressBar) {
		HPProgressBar->SetPercent(CurrentHP / MaxHP);
	}
}

void UPlayerStatusUserWidget::UpdateMP(float CurrentMP, float MaxMP)
{
	if (MPProgressBar) {
		MPProgressBar->SetPercent(CurrentMP / MaxMP);
	}
}

void UPlayerStatusUserWidget::UpdateStamina(float CurrentStamina, float MaxStamina)
{
	if (StaminaProgressBar) {
		StaminaProgressBar->SetPercent(CurrentStamina / MaxStamina);
	}
}

void UPlayerStatusUserWidget::OnPlayerStatusUswerWidgetUpdate(float NewHP, float NewMP, float NewStamina)
{
	if (auto* PlayerCharacter = Cast<AMyCharacter>(GetOwningPlayerPawn())) {
		UpdateHP(NewHP, PlayerCharacter->CharacterStatus.MaxHP);
		UpdateMP(NewMP, PlayerCharacter->CharacterStatus.MaxMP);
		UpdateStamina(NewStamina, PlayerCharacter->CharacterStatus.MaxStamina);
	}
}
