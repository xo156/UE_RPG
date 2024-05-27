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

	//���� �þ� ����
	MonsterSightConfig->SightRadius = 500.f; //Ž�� �Ÿ�
	MonsterSightConfig->LoseSightRadius = 600.0f; //Ž�� �ʱ�ȭ �Ÿ�
	MonsterSightConfig->PeripheralVisionAngleDegrees = 30.f; //�þ߰�
	MonsterSightConfig->DetectionByAffiliation.bDetectEnemies = true; //�̰� �³�?

	MonsterPerceptionComponent->ConfigureSense(*MonsterSightConfig);
	MonsterPerceptionComponent->SetDominantSense(MonsterSightConfig->GetSenseImplementation());

	//���ε�
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
			//�ϴ� �⺻ ���� ��Ÿ��
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
