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
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	//��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MonsterAttackMontageNo1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MonsterAttackMontageNo2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MonsterAttackMontageNo3;

	//���� �Ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range", meta = (AllowPrivateAccess = "true"))
	float CloseAttackRange; //�ٰŸ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range", meta = (AllowPrivateAccess = "true"))
	float MidAttackRange; //�߰Ÿ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range", meta = (AllowPrivateAccess = "true"))
	float LongAttackRange; //���Ÿ�
};
