// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWidget.h"
#include "Components/ProgressBar.h"
#include "Monster.h"
#include "Components/CanvasPanelSlot.h"

void UMonsterWidget::NativeConstruct()
{
	Super::NativeConstruct();
    // ������ ������ �� Monster�� �ùٸ��� ����Ǿ����� Ȯ��
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
        // ���� ���·� HP�� ������Ʈ�� UI�� �ùٸ��� �ʱ�ȭ�ǵ��� ��
        UpdateHP(OwnerMonster->MonsterStatus.CurrentMonsterHP, OwnerMonster->MonsterStatus.MaxMonsterHP);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("UMonsterWidget: OwningMonster is nullptr."));
    }
}
