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
	float CheckDist(class AMonsterBase* Monster);
	void MonsterAttack();

	// 타겟을 설정하는 메서드
	void SetTarget(AActor* NewTarget);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// 퍼셉션 시스템을 위한 구성 요소
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UAIPerceptionComponent* MonsterPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UAISenseConfig_Sight* MonsterSightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	AActor* TargetActor; //타겟을 저장하는 변수

	void OnTargetPerceptionUpdated(AActor* Actor, const FAIStimulus Stimulus);
	//귀가 좋고 눈이 안좋거나, 눈이 좋고 귀가 안들리는 몬스터?

};
