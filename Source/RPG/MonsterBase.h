// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterBase.generated.h"

UCLASS()
class RPG_API AMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector MonsterPosition();

	void Die();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, Category = "Monster")
	class UAnimMontage* MonsterAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Monster")
	float MonsterMaxHealth = 100.f; //몬스터 최대 체력
	float MonsterCurrentHealth = MonsterMaxHealth; //몬스터 현재 체력

	UPROPERTY(EditAnywhere, Category = "Monster")
	float MonsterAttackPower = 10.f; //몬스터 공격력

	UPROPERTY(EditAnywhere, Category = "Monster")
	float MonsterMovementSpeed = 400.f; //몬스터 이동 속도

	FVector MonsterCurrentPosition; //몬스터 위치
	FVector MonsterStartPosition;

};
