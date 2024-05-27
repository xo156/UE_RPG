// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBaseAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionSystem.h"
#include "MonsterBase.h"

AMonsterBaseAIController::AMonsterBaseAIController()
{
	MonsterPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	MonsterSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	//몬스터 시야 설정
	MonsterSightConfig->SightRadius = 500.f; //탐지 거리
	MonsterSightConfig->LoseSightRadius = 600.0f; //탐지 초기화 거리
	MonsterSightConfig->PeripheralVisionAngleDegrees = 30.f; //시야각
	MonsterSightConfig->DetectionByAffiliation.bDetectEnemies = true; //이게 맞나?

	MonsterPerceptionComponent->ConfigureSense(*MonsterSightConfig);
	MonsterPerceptionComponent->SetDominantSense(MonsterSightConfig->GetSenseImplementation());

	//바인딩
	MonsterPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterBaseAIController::OnTargetPerceptionUpdated);

}

void AMonsterBaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AMonsterBase* Monster = Cast<AMonsterBase>(GetPawn())) {
		if (CheckDist(Monster) <= 600.f) {
			TracePlayer();
		}
	}
}

void AMonsterBaseAIController::TracePlayer()
{
	if (TargetActor) {
		MoveToActor(TargetActor, 10.f);
		if (AMonsterBase* Monster = Cast<AMonsterBase>(GetPawn())) {
			if (CheckDist(Monster) <= 50.f) {
				MonsterAttack();
			}
		}
	}
}

float AMonsterBaseAIController::CheckDist(AMonsterBase* Monster)
{
	return FVector::Dist(Monster->MonsterPosition(), TargetActor->GetTargetLocation());
}

void AMonsterBaseAIController::MonsterAttack()
{
	if (TargetActor) {
		if (AMonsterBase* Monster = Cast<AMonsterBase>(GetPawn())) {
			//일단 기본 어택 몽타주
			Monster->PlayAnimMontage(Monster->MonsterAttackMontage);
		}
	}
}

void AMonsterBaseAIController::SetTarget(AActor* NewTarget)
{
	TargetActor = NewTarget;
}

void AMonsterBaseAIController::OnTargetPerceptionUpdated(AActor* Actor, const FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
		SetTarget(Actor);
	else
		SetTarget(nullptr);
}
