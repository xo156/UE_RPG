// Fill out your copyright notice in the Description page of Project Settings.


#include "MPPotion.h"
#include "MyCharacter.h"

AMPPotion::AMPPotion()
{
	MPRecoveryAmount = 50.0;
}

void AMPPotion::Use()
{
	UE_LOG(LogTemp, Log, TEXT("AMPPotion::Use()"));

	if (auto* PlayerCharacter = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
		if (PlayerCharacter->CharacterStatus.CurrentHP + MPRecoveryAmount >= PlayerCharacter->CharacterStatus.MaxHP) {

			UE_LOG(LogTemp, Log, TEXT("111 MPRecoveryAmount: %f"), MPRecoveryAmount);
			PlayerCharacter->ConsumeHPForAction(MPRecoveryAmount);
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("222 MPRecoveryAmount: %f"), MPRecoveryAmount);
			PlayerCharacter->ConsumeHPForAction(MPRecoveryAmount);
		}
	}
}
