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
    }
}

void UMonsterWidget::OnMonsterHPUpdate(float NewHP)
{
    if (OwnerMonster) {
        UpdateHP(NewHP, OwnerMonster->GetMaxMonsterHP());
    }
}

void UMonsterWidget::SetOwnerMonster(AMonster* NewOwningMonster)
{
    OwnerMonster = NewOwningMonster;
    if (OwnerMonster) {
        UpdateHP(OwnerMonster->GetMaxMonsterHP(), OwnerMonster->GetMaxMonsterHP());
        if (auto* BossMonster = Cast<ABossMonster>(OwnerMonster)) {
            BossMonster->OnMonsterUIUpdated.AddDynamic(this, &UMonsterWidget::OnMonsterHPUpdate);
            UE_LOG(LogTemp, Log, TEXT("BossMonster Cast"));
        }
        else {
            OwnerMonster->OnMonsterUIUpdated.AddDynamic(this, &UMonsterWidget::OnMonsterHPUpdate);
            UE_LOG(LogTemp, Log, TEXT("Monster Cast"));
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("OwnerMonster is nullptr"));
    }
}
