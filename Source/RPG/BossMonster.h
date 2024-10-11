// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "BossMonster.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ABossMonster : public AMonster
{
	GENERATED_BODY()
	
public:
	ABossMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	void SetUpBossWidget();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

	virtual void MonsterAttackExecute(class UAnimMontage* Montage);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void NoAction();
	void FindPlayerAround();

	//공격 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range")
	float CloseAttackRange; //근거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range")
	float MidAttackRange; //중거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range")
	float LongAttackRange; //원거리

	//상태
	int32 WanderCount = 0;
	bool bIsWander = false;

	//getter
	class UAnimMontage* GetCloseAttackMontage();
	class UAnimMontage* GetMidAttackMontage();
	class UAnimMontage* GetLongAttackMontage();
	class UAnimMontage* GetNowAttackMontage();

private:
	//몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* BossMonsterCloseAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* BossMonsterMidAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* BossMonsterLongAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* BossMonsterDieMontage;

	//위젯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UMonsterWidget> BossMonsterWidgetClass;
	class UMonsterWidget* BossMonsterWidgetInstance;

	//흔들림
	TSubclassOf<class UCameraShakeBase> BossCameraShake;

	//공격 이후 배회
	FVector PlayerLocation;
	FVector GoalLocation;
	class UAnimMontage* NowPlayMontage;
};
