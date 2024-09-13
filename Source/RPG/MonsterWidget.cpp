// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWidget.h"
#include "Components/ProgressBar.h"
#include "Monster.h"
#include "Components/CanvasPanelSlot.h"

void UMonsterWidget::NativeConstruct()
{
	Super::NativeConstruct();
    // 위젯이 생성될 때 Monster와 올바르게 연결되었는지 확인
    if (OwnerMonster) {
        OwnerMonster->OnMonsterUIUpdated.AddDynamic(this, &UMonsterWidget::OnMonsterHPUpdate);
    }
}

void UMonsterWidget::UpdateHP(float CurrentHP, float MaxHP)
{
    if (HPProgressBar) {
        HPProgressBar->SetPercent(CurrentHP / MaxHP);
        UE_LOG(LogTemp, Warning, TEXT("Updating HP ProgressBar: %f / %f"), CurrentHP, MaxHP);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("UMonsterWidget::UpdateHP: HPProgressBar is nullptr."));
    }
}

void UMonsterWidget::OnMonsterHPUpdate(float NewHP)
{
    if (OwnerMonster) {
        UpdateHP(NewHP, OwnerMonster->MonsterStatus.MaxMonsterHP);
        UE_LOG(LogTemp, Log, TEXT("UMonsterWidget::OnMonsterHPUpdate: Successfully updated HP UI."));
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("UMonsterWidget::OnMonsterHPUpdate: OwnerMonster is nullptr."));
    }
}

void UMonsterWidget::SetOwnerMonster(AMonster* NewOwningMonster)
{
    OwnerMonster = NewOwningMonster;
    if (OwnerMonster) {
        // 현재 상태로 HP를 업데이트해 UI가 올바르게 초기화되도록 함
        UpdateHP(OwnerMonster->MonsterStatus.CurrentMonsterHP, OwnerMonster->MonsterStatus.MaxMonsterHP);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("UMonsterWidget: OwningMonster is nullptr."));
    }
}
