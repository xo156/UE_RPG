// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItem.h"


void AConsumableItem::UseItem()
{
    switch (GetEffectType())
    {
    case EConsumableEffectType::Heal:
        break;
    case EConsumableEffectType::Bomb:
        break;
    case EConsumableEffectType::Buff:
        break;
    case EConsumableEffectType::DeBuff:
        break;
    default:
        break;
    }
}