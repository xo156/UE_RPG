// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWidget.h"
#include "Components/ProgressBar.h"
#include "Monster.h"

void UMonsterWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (APawn* OwnerPawn = GetOwningPlayerPawn()) {
        if (auto* Monster = Cast<AMonster>(OwnerPawn)) {
            Monster->OnMonsterUIUpdated.AddDynamic(this, &UMonsterWidget::OnMonsterHPUpdate);
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("UMonsterWidget::NativeConstruct: Failed to cast to AMonster. OwnerPawn: %s"), *OwnerPawn->GetName());
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("UMonsterWidget::NativeConstruct: GetOwningPlayerPawn() returned nullptr."));
    }
}

void UMonsterWidget::UpdateHP(float CurrentHP, float MaxHP)
{
    if (HPProgressBar) {
        HPProgressBar->SetPercent(CurrentHP / MaxHP);
        UE_LOG(LogTemp, Warning, TEXT("Updating HP ProgressBar: %f / %f"), CurrentHP, MaxHP);
    }
}

void UMonsterWidget::OnMonsterHPUpdate(float NewHP)
{
	UE_LOG(LogTemp, Warning, TEXT("UMonsterWidget::OnMonsterHPUpdate called with NewHP: %f"), NewHP);

	if (auto* Monster = Cast<AMonster>(GetOwningPlayerPawn())) {
		UpdateHP(NewHP, Monster->MonsterStatus.MaxMonsterHP);
	}
}