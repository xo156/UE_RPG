// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWidget.h"
#include "Components/ProgressBar.h"
#include "Monster.h"
#include "BossMonster.h"
#include "Components/CanvasPanelSlot.h"

void UMonsterWidget::NativeConstruct()
{
	Super::NativeConstruct();

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
    if (OwnerMonster) {
        UpdateHP(NewHP, OwnerMonster->MonsterStatus.MaxMonsterHP);
        UE_LOG(LogTemp, Log, TEXT("UMonsterWidget::OnMonsterHPUpdate: Successfully updated HP UI. New HP: %f"), NewHP);
    }
}

void UMonsterWidget::SetOwnerMonster(AMonster* NewOwningMonster)
{
    OwnerMonster = NewOwningMonster;
    if (OwnerMonster) {
        UE_LOG(LogTemp, Warning, TEXT("OwnerMonster: %s"), *OwnerMonster->GetName());
        UpdateHP(OwnerMonster->MonsterStatus.CurrentMonsterHP, OwnerMonster->MonsterStatus.MaxMonsterHP);
        UE_LOG(LogTemp, Log, TEXT("OwnerMonster->MonsterStatus.CurrentMonsterHP: %f"), OwnerMonster->MonsterStatus.CurrentMonsterHP);
        UE_LOG(LogTemp, Log, TEXT("OwnerMonster->MonsterStatus.MaxMonsterHP: %f"), OwnerMonster->MonsterStatus.MaxMonsterHP);
        if (auto* BossMonster = Cast<ABossMonster>(OwnerMonster)) {
            BossMonster->OnMonsterUIUpdated.AddDynamic(this, &UMonsterWidget::OnMonsterHPUpdate);
        }
        else {
            OwnerMonster->OnMonsterUIUpdated.AddDynamic(this, &UMonsterWidget::OnMonsterHPUpdate);
        }
    }
}
