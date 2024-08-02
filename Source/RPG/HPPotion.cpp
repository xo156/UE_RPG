
// Fill out your copyright notice in the Description page of Project Settings.


#include "HPPotion.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHPPotion::AHPPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	if (auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
		if (auto* Player = Cast<AMyCharacter>(PlayerController->GetPawn())) {
			if (Player->CharacterStatus.CurrentHP + HPRecoveryAmount >= Player->CharacterStatus.MaxHP) {
				Player->CharacterStatus.CurrentHP = Player->CharacterStatus.MaxHP;
			}
			else {
				Player->CharacterStatus.CurrentHP += HPRecoveryAmount;
			}
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, TEXT("HPPotion Used"));
		}
	}
}
