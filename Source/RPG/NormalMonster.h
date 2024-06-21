// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NormalMonster.generated.h"

UCLASS()
class RPG_API ANormalMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANormalMonster();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MonsterAttack();

	class UBehaviorTree* GetBehaviorTree() const;

	class APatrolPath* GetPatrolPath() const;

	float GetPatrolSpeed() const;
	float GetChaseSpeed() const;
	float GetHealth() const;
	float GetMaxHealth() const;
	void SetHealth(float NewHealth);
	/*void UpdateStun(float DeltaTime);
	bool GetbIsStunned();*/

	class UAnimMontage* GetNormalMonsterAttackMontage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float MaxNormalMonsterHealth = 100.f;
	float NormalMonsterHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float PatrolSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float ChaseSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float AttackRange = 100.f;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float StunDuration = 2.f;
	bool bIsStunned;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class APatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* NormalMonsterAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* WidgetComponent;

};
