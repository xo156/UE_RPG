// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToPlayerANS.h"
#include "MonsterBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMoveToPlayerANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    if (auto* Monster = Cast<AMonsterBase>(MeshComp->GetOwner())) {
        if (auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(MeshComp->GetWorld(), 0)) {
            FVector PlayerLocation = PlayerCharacter->GetActorLocation();
            FVector MonsterLocation = Monster->GetActorLocation();

            FVector DirectionToPlayer = (PlayerLocation - MonsterLocation).GetSafeNormal();
            Monster->SetActorLocation(MonsterLocation + DirectionToPlayer * Monster->GetCharacterMovement()->MaxWalkSpeed * FrameDeltaTime);
        }
    }
}
