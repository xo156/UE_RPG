// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
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
		GameInstance->LoadAllTableAndCache();
		ItemDropCache = GameInstance->GetItemDropCache();
		ItemCache = GameInstance->GetItemCache();
		MonsterDataCache = GameInstance->GetMonsterDataCache();
		CharacterDataCache = GameInstance->GetCharacterDataCache();
	}
}

void AMyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
