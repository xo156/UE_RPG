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

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void MonsterAttackExecute(int32 PatternNumber);

	virtual void WidgetFaceToPlayer() override;

	//getter
	float GetCloseRange();
	float GetMidRange();
	UAnimMontage* GetCloseAttackMontage();
	UAnimMontage* GetMidAttackMontage();
	UAnimMontage* GetLongAttackMontage();

private:
	//몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* CloseAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MidAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LongAttackMontage;

	//공격 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern", meta = (AllowPrivateAccess = "true"))
	float CloseRange = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern", meta = (AllowPrivateAccess = "true"))
	float MidRange = 400.f;
};
