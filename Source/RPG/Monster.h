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

	//델리게이트
	FOnMonsterUIUpdated OnMonsterUIUpdated;

	//상태
	bool bIsMonsterAttack = false;
	bool bIsMonsterDead = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//데이터
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

	//공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UMonsterAttackComponent* MonsterAttackComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* MonsterAttackCollision0;
	TArray<class UCapsuleComponent*> MonsterAttackCollisions;
	TArray<AActor*> OverlapActors; //몬스터가 때릴 때 콜리전 검출하기
	float Damage = 0.f;

	//몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MonsterAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MonsterDieMontage;

	//위젯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UMonsterWidgetComponent* MonsterWidgetComponent;

	//아이템 드랍
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess="true"))
	TSubclassOf<class ADropItem> DropItemClass;
	TMap<int32, FDropRate*> ItemCache;

	//흔들림
	TSubclassOf<class UCameraShakeBase> CameraShake;

	class UDataTable* MonsterDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess="true"))
	int32 MonsterID;
};
