// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_WaitForNext.h"
#include "BossMonsterAIC.h"
#include "BossMonster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
				SetAroundLocation(PlayerCharacter, BossMonster);
				WaitForNextActionTime = BossMonster->GetWaitForNextActionTime();

				ElapsedTime = 0.f;
				return EBTNodeResult::InProgress;
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_WaitForNext::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (auto* BossMonsterAIC = Cast<ABossMonsterAIC>(OwnerComp.GetAIOwner())) {
		if (auto* BossMonster = Cast<ABossMonster>(BossMonsterAIC->GetPawn())) {
			if (auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {
				BossMonsterAIC->MoveToLocation(AroundLocation);

				//배회하는 시간 계산
				ElapsedTime += DeltaSeconds;
				if (ElapsedTime >= WaitForNextActionTime) {
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
			}
		}
	}
}

void UBTTask_WaitForNext::SetAroundLocation(AActor* PlayerCharacter, ABossMonster* BossMonster)
{
	if (PlayerCharacter && BossMonster) {
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		FVector RandomOffset = UKismetMathLibrary::RandomUnitVector() * BossMonster->GetPlayerAroundRadius();
		AroundLocation = PlayerLocation + RandomOffset;
		AroundLocation.Z = BossMonster->GetActorLocation().Z;
	}
}
