// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "BossMonster.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossMonsterUIUpdated, float, NewHP);

UCLASS()
class RPG_API ABossMonster : public AMonster
{
	GENERATED_BODY()
	
public:
	ABossMonster();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void MonsterAttackExecute(int32 PatternNumber);

	//getter
	float GetCloseRange();
	float GetMidRange();
	UAnimMontage* GetCloseAttackMontage();
	UAnimMontage* GetMidAttackMontage();
	UAnimMontage* GetLongAttackMontage();

	//델리게이트
	FOnBossMonsterUIUpdated OnBossMonsterUIUpdated;

	//구조체
	virtual void ConsumeHPForAction(float HPCost);

private:
	//몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* CloseAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MidAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LongAttackMontage;

	//공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float CloseRange = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float MidRange = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* MonsterAttackCollisionComponent1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* MonsterAttackCollisionComponent2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* MonsterAttackCollisionComponent3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* MonsterAttackCollisionComponent4;

	class UMonsterWidget* MonsterWidgetInstance;
};
