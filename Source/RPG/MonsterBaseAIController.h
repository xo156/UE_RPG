// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MonsterBaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AMonsterBaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterBaseAIController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TracePlayer();
	float MonsterToPlayerDist(class AMonsterBase* Monster);
	void MonsterAttack();

	// Ÿ���� �����ϴ� �޼���
	void SetTarget(AActor* NewTarget);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// �ۼ��� �ý����� ���� ���� ���
	UPROPERTY(VisibleAnywhere,  Category = "AI")
	class UAIPerceptionComponent* MonsterPerceptionComponent;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAISenseConfig_Sight* MonsterSightConfig;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAISenseConfig_Hearing* MonsterHearingConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	AActor* TargetActor; //Ÿ���� �����ϴ� ����

	void OnTargetPerceptionUpdated(AActor* Actor, const FAIStimulus Stimulus);
	//�Ͱ� ���� ���� �����ų�, ���� ���� �Ͱ� �ȵ鸮�� ����?

	class AMonsterBase* GetMonsterPawn() const;

};
