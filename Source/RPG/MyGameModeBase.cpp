// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "BossMonster.h"
#include "DataTableGameInstance.h"
#include "Kismet/GameplayStatics.h"

AMyGameModeBase::AMyGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance) {
		GameInstance->LoadAllTables();
		ItemDropCache = GameInstance->GetItemDropCache();
	}
}

void AMyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
