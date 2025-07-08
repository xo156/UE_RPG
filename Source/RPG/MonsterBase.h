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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//�غ�
	void InitMonsterInfo(const struct FMonsterData& InData);

	//����
	void MonsterAttackStart(int32 AttackIndex);
	virtual void MonsterAttackExecute(int32 AttackIndex);
	void UpdateValidPatternIndexes(float Distance, float HPRatio);
	bool bIsValidAttackPatternIndex(int32 Index) const;
	void EnableCollisionName(FName TargetName);
	void DisableCollisionNAme(FName TargetName);
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void MonsterAttackEnd();
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, 
						 bool bFromSweep, const FHitResult& SweepResult);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void DroppedItem();
	void DieMonster();
	void OnDieMontageEnded(UAnimMontage* NowPlayMontage, bool bInterrupted);
	void ApplyDamageToActor(AActor* ActorToDamage, UPrimitiveComponent* OtherComponent);

	//getter
	class UBehaviorTree* GetBehaviorTree() const;
	class APatrolPath* GetPatrolPath() const;
	TArray<class UMonsterAttackPatternDataAsset*>& GetAttackPatterns();
	TArray<int32> GetValidPatternIndexes() const;
	TArray<AActor*>& GetOverlapActors();
	class UHPActorComponent* GetHPActorComponent() const;
	class UMonsterStateMachineComponent* GetMonsterStateMachineComponent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class APatrolPath* PatrolPath;

	//������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	int32 MonsterID;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	FName MonsterName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float MonsterDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TArray<int32> MonsterDropItemIDs;

	//����
	TArray<AActor*> OverlapActors; //���Ͱ� ���� �� �ݸ��� �����ϱ�
	TArray<class UMonsterAttackPatternDataAsset*> MonsterAttackPatterns; //���� ���� ���� ���
	TArray<int32> ValidAttackPatternIndexes; //���� ������ ������ �ε���

	//�ڿ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource", meta = (AllowPrivateAccess))
	class UHPActorComponent* HPActorComponent;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StateMachine", meta = (AllowPrivateAccess = "true"))
	class UMonsterStateMachineComponent* MonsterStateMachineComponent;

	//��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MonsterAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MonsterDieMontage;

	//������ ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess="true"))
	TSubclassOf<class ADropItem> DropItemClass;
	TMap<int32, struct FDropRate*> ItemCache;
};
