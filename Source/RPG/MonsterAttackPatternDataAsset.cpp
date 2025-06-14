// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAttackPatternDataAsset.h"

bool UMonsterAttackPatternDataAsset::bIsValidCondition(float Distance, float HP)
{
	return Distance <= RequiredDistance && HP <= RequiredHP;
}
