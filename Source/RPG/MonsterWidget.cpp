// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWidget.h"
#include "Components/ProgressBar.h"
#include "Monster.h"
#include "Components/CanvasPanelSlot.h"

void UMonsterWidget::NativeConstruct()
{
	Super::NativeConstruct();

    /*if (auto* OwnerPawn = GetOwningPlayerPawn()) {
        if (auto* Monster = Cast<AMonster>(OwnerPawn)) {
            UE_LOG(LogTemp, Log, TEXT("UMonsterWidget::NativeConstruct: Successfully casted to AMonster. OwnerPawn: %s"), *OwnerPawn->GetName());
            Monster->OnMonsterUIUpdated.AddDynamic(this, &UMonsterWidget::OnMonsterHPUpdate);
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("UMonsterWidget::NativeConstruct: Failed to cast to AMonster. OwnerPawn: %s"), *OwnerPawn->GetName());
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("UMonsterWidget::NativeConstruct: GetOwningPlayerPawn() returned nullptr."));
    }*/

    // 위젯이 생성될 때 Monster와 올바르게 연결되었는지 확인
    if (OwnerMonster) {
        OwnerMonster->OnMonsterUIUpdated.AddDynamic(this, &UMonsterWidget::OnMonsterHPUpdate);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("UMonsterWidget::NativeConstruct: OwnerMonster is nullptr"));
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
	UE_LOG(LogTemp, Warning, TEXT("UMonsterWidget::OnMonsterHPUpdate called with NewHP: %f"), NewHP);

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
        UE_LOG(LogTemp, Log, TEXT("UMonsterWidget: OwningMonster set to %s"), *OwnerMonster->GetName());
        // 현재 상태로 HP를 업데이트해 UI가 올바르게 초기화되도록 함
        UpdateHP(OwnerMonster->MonsterStatus.CurrentMonsterHP, OwnerMonster->MonsterStatus.MaxMonsterHP);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("UMonsterWidget: OwningMonster is nullptr."));
    }
}
