// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBaseAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
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

	//몬스터 청각 설정
	MonsterHearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("MonsterHearingConfig"));
	MonsterHearingConfig->HearingRange = 700.0f;
	MonsterHearingConfig->SetMaxAge(5.0f);
	MonsterSightConfig->DetectionByAffiliation.bDetectEnemies = true;

	MonsterPerceptionComponent->ConfigureSense(*MonsterSightConfig);
	MonsterPerceptionComponent->ConfigureSense(*MonsterHearingConfig);
	MonsterPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());

	//바인딩
	MonsterPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterBaseAIController::OnTargetPerceptionUpdated);

}

void AMonsterBaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TracePlayer();
}

void AMonsterBaseAIController::TracePlayer()
{
	AMonsterBase* Monster = GetMonsterPawn();
	if (Monster && TargetActor)	{
		float DistanceToTarget = MonsterToPlayerDist(Monster);
		if (DistanceToTarget <= 50.0f) {
			MonsterAttack();
		}
		else if (DistanceToTarget <= 500.0f) {
			MoveToActor(TargetActor, 10.0f);
		}
	}
}

float AMonsterBaseAIController::MonsterToPlayerDist(AMonsterBase* Monster)
{
	if (Monster && TargetActor) {
		return FVector::Dist(Monster->GetActorLocation(), TargetActor->GetActorLocation());
	}
	return 0.0f;
}

void AMonsterBaseAIController::MonsterAttack()
{
	if (TargetActor) {
		if (AMonsterBase* Monster = GetMonsterPawn()) {
			if (UAnimInstance* AnimInstance = Monster->GetMesh()->GetAnimInstance()) {
				AnimInstance->Montage_Play(Monster->MonsterAttackMontage);
			}
		}
	}
}

void AMonsterBaseAIController::SetTarget(AActor* NewTarget)
{
	TargetActor = NewTarget;
}

void AMonsterBaseAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterBaseAIController::OnTargetPerceptionUpdated(AActor* Actor, const FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
		SetTarget(Actor);
	else
		SetTarget(nullptr);
}

AMonsterBase* AMonsterBaseAIController::GetMonsterPawn() const
{
	return Cast<AMonsterBase>(GetPawn());
}
