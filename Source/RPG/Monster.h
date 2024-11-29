// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DropRate.h"
#include "MonsterData.h"
#include "Monster.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonsterUIUpdated, float, NewHP);

UCLASS()
class RPG_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetMonsterInfo();

	void MonsterAttackStart();
	virtual void MonsterAttackExecute();
	void MonsterAttackEnd();
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, 
						 bool bFromSweep, const FHitResult& SweepResult);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void ConsumeHPForAction(float HPCost); void DroppedItem();
	void DieMonster();
	void OnDieMontageEnded(UAnimMontage* NowPlayMontage, bool bInterrupted);
	void ApplyDamageToActor(AActor* ActorToDamage, UPrimitiveComponent* OtherComponent);

	//getter
	class UBehaviorTree* GetBehaviorTree() const;
	class APatrolPath* GetPatrolPath() const;
	class UAnimMontage* GetMonsterAttackMontage() const;
	TArray<AActor*>& GetOverlapActors();
	class UWidgetComponent* GetMonsterWidgetComponent() const;
	UCapsuleComponent* GetAttackCollisionComponent(FName AttackCollisionFName) const;
	float GetMaxMonsterHP();

	//setter
	void SetMonsterAttackCollision(class UCapsuleComponent* AttackCollision);

	//��������Ʈ
	FOnMonsterUIUpdated OnMonsterUIUpdated;

	//����
	bool bIsMonsterAttack = false;
	bool bIsMonsterDead = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//������
	float MaxMonsterHP;
	float CurrentMonsterHP;
	float MonsterDamage;
	TArray<int32> MonsterDropItemIDS;

private:
	//AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class APatrolPath* PatrolPath;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UMonsterAttackComponent* MonsterAttackComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* MonsterAttackCollision0;
	TArray<class UCapsuleComponent*> MonsterAttackCollisions;
	TArray<AActor*> OverlapActors; //���Ͱ� ���� �� �ݸ��� �����ϱ�
	float Damage = 0.f;

	//��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MonsterAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MonsterDieMontage;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UMonsterWidgetComponent* MonsterWidgetComponent;

	//������ ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess="true"))
	TSubclassOf<class ADropItem> DropItemClass;
	TMap<int32, FDropRate*> ItemCache;

	//��鸲
	TSubclassOf<class UCameraShakeBase> CameraShake;

	class UDataTable* MonsterDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess="true"))
	int32 MonsterID;
};
