// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_WaitForNext.h"
#include "BossMonsterAIC.h"
#include "BossMonster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTask_WaitForNext::UBTTask_WaitForNext()
{
	bNotifyTick = true;
	NodeName = TEXT("Wait For Next");

}

EBTNodeResult::Type UBTTask_WaitForNext::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* BossMonsterAIC = Cast<ABossMonsterAIC>(OwnerComp.GetAIOwner())) {
		if (auto* BossMonster = Cast<ABossMonster>(BossMonsterAIC->GetPawn())) {
			if (auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {
				ElapsedTime = 0.f;
				WaitForNextActionTime = BossMonster->GetWaitForNextActionTime();
				SetAroundLocation(PlayerCharacter, BossMonster);
				return EBTNodeResult::InProgress;
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_WaitForNext::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ElapsedTime += DeltaSeconds;
	if (ElapsedTime >= WaitForNextActionTime) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	if (auto* BossMonsterAIC = Cast<ABossMonsterAIC>(OwnerComp.GetAIOwner())) {
		if (auto* BossMonster = Cast<ABossMonster>(BossMonsterAIC->GetPawn())) {
			if (ElapsedTime >= BossMonster->GetWaitForNextActionTime()) {
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return;
			}

			if (auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {
				if (FMath::Fmod(ElapsedTime, 1.0f) < DeltaSeconds) {
					SetAroundLocation(PlayerCharacter, BossMonster);
				}
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(BossMonsterAIC, AroundLocation);
			}
		}
	}
}

void UBTTask_WaitForNext::SetAroundLocation(AActor* PlayerCharacter, ABossMonster* BossMonster)
{
	if (PlayerCharacter && BossMonster) {
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		float MoveRadius = FMath::FRandRange(MinAroundRadius, MaxAroundRadius);
		FVector RandomOffset = UKismetMathLibrary::RandomUnitVector() * MoveRadius;
		AroundLocation = PlayerLocation + RandomOffset;
		AroundLocation.Z = PlayerLocation.Z;
	}
}
