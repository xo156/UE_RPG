
// Fill out your copyright notice in the Description page of Project Settings.


#include "HPPotion.h"
#include "MyCharacter.h"

// Sets default values
AHPPotion::AHPPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HPRecoveryAmount = 30.f;
}

// Called when the game starts or when spawned
void AHPPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHPPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHPPotion::Use()
{
	UE_LOG(LogTemp, Log, TEXT("AHPPotion::Use()"));

	if (auto* PlayerCharacter = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
		if (PlayerCharacter->CharacterStatus.CurrentHP + HPRecoveryAmount >= PlayerCharacter->CharacterStatus.MaxHP) {
			//PlayerCharacter->CharacterStatus.CurrentHP = PlayerCharacter->CharacterStatus.MaxHP;

			UE_LOG(LogTemp, Log, TEXT("111 HPRecoveryAmount: %f"), HPRecoveryAmount);
			PlayerCharacter->ConsumeHPForAction(HPRecoveryAmount);
		}
		else {
			//PlayerCharacter->CharacterStatus.CurrentHP += HPRecoveryAmount;
			UE_LOG(LogTemp, Log, TEXT("222 HPRecoveryAmount: %f"), HPRecoveryAmount);
			PlayerCharacter->ConsumeHPForAction(HPRecoveryAmount);
		}
	}
}